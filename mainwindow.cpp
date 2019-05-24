#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aesencrypt.h"
#include <QMessageBox>
#include <QDebug>
//#include "qaesencryption.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

AesEncrypt::AesPadding paddingtype(int index){
    switch (index) {
    case 0:
        return AesEncrypt::ZEROPadding;
        break;
    case 1:
        return AesEncrypt::PKCS5Padding;
        break;
    case 2:
        return AesEncrypt::PKCS7Padding;
        break;
    case 3:
        return AesEncrypt::ISO10126Padding;
        break;
    case 4:
        return AesEncrypt::NoPadding;
        break;
    default:
        break;
    }
    return AesEncrypt::NoPadding;
}

//加密
void MainWindow::on_pushButton_clicked()
{
    if(ui->textEdit->toPlainText().isEmpty()){
        QMessageBox::warning(this, "ERROR", "NO encrypt string");
    }
    if(ui->lineEdit->text().isEmpty()){
        QMessageBox::warning(this, "ERROR", "NO encrypt key");
    }


    std::string str;
    switch (ui->comboBox->currentIndex()) {
    case 0://ECB
        str = AesEncrypt::AesEcbEncrypt(ui->textEdit->toPlainText().toStdString(),
                                        ui->lineEdit->text().toStdString(), paddingtype(ui->comboBox_2->currentIndex()));
        break;
    case 1://CBC
        str = AesEncrypt::AesCbcEncrypt(ui->textEdit->toPlainText().toStdString(),
                                        ui->lineEdit->text().toStdString(),
                                        ui->lineEdit_2->text().toStdString(),
                                        paddingtype(ui->comboBox_2->currentIndex()));
        break;
    default:
        break;
    }

    qDebug()<<__FUNCTION__<<"Aes"<<ui->textEdit->toPlainText()<<"to"<<QByteArray(str.c_str());
    QByteArray data = QByteArray(str.c_str());//.toBase64(QByteArray::Base64Encoding);
    ui->textEdit_2->setText(QString::fromLocal8Bit(data));
}

//解密
void MainWindow::on_pushButton_2_clicked()
{
    QByteArray data = ui->textEdit_2->toPlainText().toLocal8Bit();
    qDebug()<<__FUNCTION__<<data<<ui->lineEdit->text();
    std::string str;
    switch (ui->comboBox->currentIndex()) {
    case 0:
        str = AesEncrypt::AesEcbDecrypt(data.toStdString(),
                                        ui->lineEdit->text().toStdString(),
                                        paddingtype(ui->comboBox_2->currentIndex()));
        break;
    case 1:
        str = AesEncrypt::AesCbcDecrypt(data.toStdString(),
                                        ui->lineEdit->text().toStdString(),
                                        ui->lineEdit_2->text().toStdString(),
                                        paddingtype(ui->comboBox_2->currentIndex()));
        break;
    default:
        break;
    }

    qDebug()<<__FUNCTION__<<str.c_str();
    qDebug()<<__FUNCTION__<<"Aes"<<data<<"to"<<QByteArray(str.c_str());
    ui->textEdit->setText(str.c_str());

}
