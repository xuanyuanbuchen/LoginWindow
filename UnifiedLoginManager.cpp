#include "UnifiedLoginManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <qtcpserver.h>
#include <qrandom.h>
#include "SqlTools.h"
#include "smtpclient.h"
#include "mimemessage.h"
#include "mimetext.h"
#include <qjsonarray.h>
#include <qfile.h>

UnifiedLoginManager::UnifiedLoginManager(QObject* parent)
    : QObject(parent), stop(false) 
{
    // 初始化线程池
    const size_t threadCount = std::thread::hardware_concurrency();
    for (size_t i = 0; i < threadCount; ++i)
    {
        workers.emplace_back(&UnifiedLoginManager::WorkerThread, this);
    }
}

UnifiedLoginManager::~UnifiedLoginManager() 
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers)
    {
        worker.join();
    }
}

void UnifiedLoginManager::startHttpServer(quint16 port) 
{
    // 设置 HTTP 端点

    // 根路由返回登录页面
    httpServer.route("/", [](const QHttpServerRequest& request, QHttpServerResponder& responder) {
        Q_UNUSED(request);

        // 尝试打开登录页面文件
        QFile file(":/static/static/login.html");  // 使用Qt资源系统路径

        if (!file.exists()) {
            QHttpHeaders headers;
            headers.append("Content-Type", "application/json");
            responder.write(QJsonDocument({
                {"status", "error"},
                {"message", "Login page not found"}
                }).toJson(), headers, QHttpServerResponder::StatusCode::NotFound);
            return;
        }

        if (!file.open(QIODevice::ReadOnly)) {
            QHttpHeaders headers;
            headers.append("Content-Type", "application/json");
            responder.write(QJsonDocument({
                {"status", "error"},
                {"message", "Cannot open login page"}
                }).toJson(), headers, QHttpServerResponder::StatusCode::InternalServerError);
            return;
        }

        // 成功读取文件
        QByteArray fileData = file.readAll();
        file.close();

        QHttpHeaders headers;
        headers.append("Content-Type", "text/html");
        responder.write(fileData, headers);
        });

    // 静态文件路由（带完整错误处理）
    httpServer.route("/register", [](const QHttpServerRequest& request, QHttpServerResponder& responder) {
        Q_UNUSED(request);

        // 尝试打开登录页面文件
        QFile file(":/static/static/register.html");  // 使用Qt资源系统路径

        if (!file.exists()) {
            QHttpHeaders headers;
            headers.append("Content-Type", "application/json");
            responder.write(QJsonDocument({
                {"status", "error"},
                {"message", "register page not found"}
                }).toJson(), headers, QHttpServerResponder::StatusCode::NotFound);
            return;
        }

        if (!file.open(QIODevice::ReadOnly)) {
            QHttpHeaders headers;
            headers.append("Content-Type", "application/json");
            responder.write(QJsonDocument({
                {"status", "error"},
                {"message", "Cannot open register page"}
                }).toJson(), headers, QHttpServerResponder::StatusCode::InternalServerError);
            return;
        }

        // 成功读取文件
        QByteArray fileData = file.readAll();
        file.close();

        QHttpHeaders headers;
        headers.append("Content-Type", "text/html");
        responder.write(fileData, headers);
        });

    // 登录API路由
    httpServer.route("/e/", QHttpServerRequest::Method::Post,
        [this](const QHttpServerRequest& request, QHttpServerResponder& responder) {
            const auto body = QJsonDocument::fromJson(request.body()).object();
            const QString username = body["username"].toString();
            const QString password = body["password"].toString();

            QJsonObject response;
            response["status"] = "success";
            response["userType"] = 0;

            QJsonDocument json(response);
            responder.write(json, CorsHelper::corsHeaders());

        });

    // OPTIONS 路由
    httpServer.route("/e/", QHttpServerRequest::Method::Options,
        [](const QHttpServerRequest&, QHttpServerResponder& responder) {
            CorsHelper::handleOptions(responder);
        });


    // 创建 QTcpServer 并绑定到 QHttpServer
    auto tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port) || !httpServer.bind(tcpServer)) 
    {
        qCritical() << "无法启动 HTTP 服务器，端口：" << port;
        delete tcpServer;
        return;
    }

    qDebug() << "HTTP 服务器已启动，监听端口：" << tcpServer->serverPort();
}


template<typename F>
void UnifiedLoginManager::AddTask(F&& task)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::make_unique<TaskImpl<F>>(std::forward<F>(task)));
    }
    condition.notify_one();
}

void UnifiedLoginManager::WorkerThread()
{
    while (true)
    {
        std::unique_ptr<ITask> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) return;
            task = std::move(tasks.front()); 
            tasks.pop();
        }
        task->run();  //
    }
}

bool UnifiedLoginManager::localPasswordLogin(const QString& username, const QString& password, int& userType) 
{
    QMutexLocker locker(&loginMutex);

    // 使用 SqlTools 验证用户名和密码
    int result = SqlTools::Login_Account_Password_Check(username.toStdString(), password.toStdString());

    if (result == 1) 
    {
        userType = 1; // 管理员
        return true;
    }
    else if (result == 2)
    {
        userType = 2; // 普通用户
        return true;
    }

    return false; // 登录失败
}


bool UnifiedLoginManager::localEmailLogin(const QString& email, const QString& code, int& userType) 
{
    QMutexLocker locker(&loginMutex);

    // 使用 SqlTools 验证邮箱和验证码
    int result = SqlTools::Login_Email_Code_Check(email.toStdString(), code.toStdString());

    if (result == 1) 
    {
        userType = 1; // 管理员
        return true;
    }
    else if (result == 2) 
    {
        userType = 2; // 普通用户
        return true;
    }

    return false; // 验证失败
}


void UnifiedLoginManager::localLogout(const QString& username) 
{
    QMutexLocker locker(&loginMutex);

    // 模拟清理本地登录状态
    qDebug() << "用户" << username << "已登出";
}



void UnifiedLoginManager::remotePasswordLogin(const QString& username, const QString& password, QHttpServerResponder&& responder) {
    int userType = 0;
    if (localPasswordLogin(username, password, userType)) 
    {
        QJsonObject response;
        response["status"] = "success";
        response["userType"] = userType;
        responder.write(QJsonDocument(response).toJson(), "application/json");
    }
    else 
    {
        QJsonObject response;
        response["status"] = "failure";
        response["message"] = "Invalid username or password";
        responder.write(QJsonDocument(response).toJson(), "application/json");
    }
}

void UnifiedLoginManager::remoteEmailLogin(const QString& email, const QString& code, QHttpServerResponder&& responder) {
    int userType = 0;

    if (localEmailLogin(email, code, userType)) 
    {
        QJsonObject response;
        response["status"] = "success";
        response["userType"] = userType;
        responder.write(QJsonDocument(response).toJson(), "application/json");
    }
    else 
    {
        QJsonObject response;
        response["status"] = "failure";
        response["message"] = "Invalid email or verification code";
        responder.write(QJsonDocument(response).toJson(), "application/json");
    }
}

void UnifiedLoginManager::remoteLogout(const QString& token, QHttpServerResponder&& responder) {
    QMutexLocker locker(&loginMutex);

    // 检查 Token 是否存在
    auto it = remoteTokens.find(token);
    if (it != remoteTokens.end()) 
    {
        // 清理 Token
        remoteTokens.erase(it);

        // 返回成功响应
        QJsonObject response;
        response["status"] = "success";
        response["message"] = "Logout successful";
        responder.write(QJsonDocument(response).toJson(), "application/json");

        qDebug() << "远程用户登出成功，Token：" << token;
    }
    else 
    {
        // 返回失败响应
        QJsonObject response;
        response["status"] = "failure";
        response["message"] = "Invalid token";
        responder.write(QJsonDocument(response).toJson(), "application/json");

        qDebug() << "远程用户登出失败，无效的 Token：" << token;
    }
}



void UnifiedLoginManager::sendLocalVerificationCode(const QString& email) 
{
    // 生成随机验证码
    QString code = QString::number(QRandomGenerator::global()->bounded(1000000)).rightJustified(6, '0');

    // 使用 SqlTools 存储验证码
    if (!SqlTools::Add_VerificationCode(email.toStdString(), code.toStdString())) 
    {
        qDebug() << "存储验证码失败，邮箱：" << email;
        return;
    }

    // 使用 SmtpClient 发送邮件
    SmtpClient smtp("smtp.qq.com", 465, SmtpClient::SslConnection); // 替换为您的 SMTP 服务器地址和端口
    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) 
    {
        qDebug() << "无法连接到 SMTP 服务器";
        return;
    }

    smtp.login("3188057460@qq.com", "zpekmzpfrgkzddia"); // 替换为您的邮箱和密码
    if (!smtp.waitForAuthenticated()) 
    {
        qDebug() << "SMTP 登录失败";
        return;
    }

    MimeMessage message;
    message.setSender(EmailAddress("3188057460@qq.com", "轩辕步尘")); // 替换为您的邮箱和名称
    message.addRecipient(EmailAddress(email));
    message.setSubject("登录验证码");

    MimeText text;
    text.setText("您的验证码是：" + code);
    message.addPart(&text);

    smtp.sendMail(message);
    if (!smtp.waitForMailSent())
    {
        qDebug() << "邮件发送失败";
        return;
    }

    smtp.quit();

    qDebug() << "验证码已发送到邮箱：" << email << "，验证码：" << code;
}




void UnifiedLoginManager::sendRemoteVerificationCode(const QString& email, QHttpServerResponder&& responder) 
{
    // 生成随机验证码
    QString code = QString::number(QRandomGenerator::global()->bounded(1000000)).rightJustified(6, '0');

    // 使用 SqlTools 存储验证码
    if (!SqlTools::Add_VerificationCode(email.toStdString(), code.toStdString())) 
    {
        qDebug() << "存储验证码失败，邮箱：" << email;
        QJsonObject response;
        response["status"] = "failure";
        response["message"] = "存储验证码失败";
        responder.write(QJsonDocument(response).toJson(), "application/json");
        return;
    }

    // 使用 SmtpClient 发送邮件
    SmtpClient smtp("smtp.example.com", 465, SmtpClient::SslConnection); // 替换为您的 SMTP 服务器地址和端口
    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) 
    {
        qDebug() << "无法连接到 SMTP 服务器";
        QJsonObject response;
        response["status"] = "failure";
        response["message"] = "无法连接到 SMTP 服务器";
        responder.write(QJsonDocument(response).toJson(), "application/json");
        return;
    }

    smtp.login("your_email@example.com", "your_password"); // 替换为您的邮箱和密码
    if (!smtp.waitForAuthenticated())
    {
        qDebug() << "SMTP 登录失败";
        QJsonObject response;
        response["status"] = "failure";
        response["message"] = "SMTP 登录失败";
        responder.write(QJsonDocument(response).toJson(), "application/json");
        return;
    }

    MimeMessage message;
    message.setSender(EmailAddress("your_email@example.com", "UnifiedLoginManager")); // 替换为您的邮箱和名称
    message.addRecipient(EmailAddress(email));
    message.setSubject("登录验证码");

    MimeText text;
    text.setText("您的验证码是：" + code);
    message.addPart(&text);

    smtp.sendMail(message);
    if (!smtp.waitForMailSent())
    {
        qDebug() << "邮件发送失败";
        QJsonObject response;
        response["status"] = "failure";
        response["message"] = "邮件发送失败";
        responder.write(QJsonDocument(response).toJson(), "application/json");
        return;
    }

    smtp.quit();

    // 返回成功响应
    QJsonObject response;
    response["status"] = "success";
    response["message"] = "验证码已发送";
    responder.write(QJsonDocument(response).toJson(), "application/json");

    qDebug() << "验证码已发送到邮箱：" << email << "，验证码：" << code;
}



void UnifiedLoginManager::logAction(const QString& action, const QString& username, const QString& userType, const QString& method, const QString& result) 
{
    qDebug() << "Action:" << action << ", Username:" << username << ", UserType:" << userType << ", Method:" << method << ", Result:" << result;
}

