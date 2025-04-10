#include "CustomerDetailDialog.h"
#include <QFileDialog>
#include <QPixmap>

CustomerDetailDialog::CustomerDetailDialog(QWidget* parent)
    : QDialog(parent), avatarPath(":/res/default.jpg") // ��ʼ��ͷ��·��ΪĬ��ͼƬ
{
    ui.setupUi(this);

    // ���� acceptBtn �� cancelBtn �ĵ���źŵ��ۺ���
    connect(ui.acceptBtn, &QPushButton::clicked, this, &CustomerDetailDialog::onAcceptBtnClicked);
    connect(ui.cancelBtn, &QPushButton::clicked, this, &CustomerDetailDialog::onCancelBtnClicked);

    // ����˫���źŵ��ۺ���
    connect(ui.customerPicture, &ClickableLabel::doubleClicked, this, &CustomerDetailDialog::onCustomerPictureDoubleClicked);
}

CustomerDetailDialog::~CustomerDetailDialog()
{
}

void CustomerDetailDialog::onAcceptBtnClicked()
{
    // �ռ��û����������
    QString id = ui.customerIDEdit->text();
    QString birthDate = ui.customerBirthDateEdit->text();
    QString logDate = ui.customerLogDateEdit->text();
    QString email = ui.customerEmailEdit->text();
    QString password = ui.customerPasswordEdit->text();
    QString description = ui.customerTextEdit->toPlainText();

    // �����źţ������ݷ��ظ��ⲿ������
    emit dataAccepted(id, birthDate, logDate, email, password, description, avatarPath);

    // �رմ���
    close();
}

void CustomerDetailDialog::onCancelBtnClicked()
{
    // �رմ��ڲ�ȡ�������޸�
    close();
}

void CustomerDetailDialog::onCustomerPictureDoubleClicked()
{
    // �����ļ�ѡ�񴰿�
    QString filePath = QFileDialog::getOpenFileName(this, tr("ѡ��ͼƬ"), "", tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp)"));
    if (!filePath.isEmpty())
    {
        // ���ز�������ͼƬ
        QPixmap newPixmap(filePath);
        if (!newPixmap.isNull())
        {
            ui.customerPicture->setPixmap(newPixmap);
            avatarPath = filePath; // ����ѡ���ͼƬ·��
        }
    }
}
