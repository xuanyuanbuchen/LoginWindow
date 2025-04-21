#pragma once

#include <QObject>
#include <QHttpServer>
#include <QMap>
#include <QDateTime>
#include <QMutex>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <unordered_map>

struct ITask {
    virtual void run() = 0;
    virtual ~ITask() = default;
};

template<typename F>
struct TaskImpl : ITask {
    F func;
    TaskImpl(F&& f) : func(std::move(f)) {}
    void run() override { func(); }
};

class UnifiedLoginManager : public QObject 
{
    Q_OBJECT

public:
    static UnifiedLoginManager& instance() 
    {
        static UnifiedLoginManager instance; // 单例实例
        return instance;
    }

    // 禁用拷贝和赋值
    UnifiedLoginManager(const UnifiedLoginManager&) = delete;
    UnifiedLoginManager& operator=(const UnifiedLoginManager&) = delete;

private:
    explicit UnifiedLoginManager(QObject* parent = nullptr);
    ~UnifiedLoginManager();

public:

    // 启动 HTTP 服务器
    void startHttpServer(quint16 port);

    // 本地登录接口
    bool localPasswordLogin(const QString& username, const QString& password, int& userType);
    bool localEmailLogin(const QString& email, const QString& code, int& userType);

    // 本地登出接口
    void localLogout(const QString& username);

    // 远程登录接口
    void remotePasswordLogin(const QString& username, const QString& password, QHttpServerResponder&& responder);
    void remoteEmailLogin(const QString& email, const QString& code, QHttpServerResponder&& responder);

    // 远程登出接口
    void remoteLogout(const QString& token, QHttpServerResponder&& responder);

    // 邮箱验证码管理
    void sendLocalVerificationCode(const QString& email);
    void sendRemoteVerificationCode(const QString& email, QHttpServerResponder&& responder);

    // 日志记录
    void logAction(const QString& action, const QString& username, const QString& userType, const QString& method, const QString& result);


private:
    // 线程池相关
    std::queue<std::unique_ptr<ITask>> tasks;

    template<typename F>
    void AddTask(F&& task);
    void WorkerThread();

    std::vector<std::thread> workers;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

    // 登录状态管理
    QMap<QString, QString> localVerificationCodes;
    QMap<QString, QDateTime> localCodeExpiry;
    QMap<QString, QString> remoteTokens;
    QMutex loginMutex;

    // HTTP 服务器
    QHttpServer httpServer;
};

class CorsHelper
{
public:
    static QHttpHeaders corsHeaders()
    {
        QHttpHeaders headers;
        headers.replaceOrAppend(QHttpHeaders::WellKnownHeader::AccessControlAllowOrigin, "*");
        headers.replaceOrAppend(QHttpHeaders::WellKnownHeader::AccessControlAllowMethods, "GET, POST, OPTIONS");
        headers.replaceOrAppend(QHttpHeaders::WellKnownHeader::AccessControlAllowHeaders, "Content-Type");
        return headers;
    }

    static void handleOptions(QHttpServerResponder& responder)
    {
        responder.write(corsHeaders());
    }
};

