#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Controller* controller, QWidget *parent)
    : controller(controller), QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QString>
void MainWindow::on_pushButton_clicked()
{
    string str1=ui->lineEdit->text().toStdString();
    string str2=ui->plainTextEdit->toPlainText().toStdString();
    string out=controller->encrypt(std::move(str1),std::move(str2));

    ui->plainTextEdit_2->setPlainText(QString::fromStdString(out));
}


void MainWindow::on_pushButton_2_clicked()
{
    string str1=ui->lineEdit->text().toStdString();
    string str2=ui->plainTextEdit->toPlainText().toStdString();
    string out=controller->decrypt(std::move(str1),std::move(str2));
    ui->plainTextEdit_2->setPlainText(QString::fromStdString(out));
}


void MainWindow::on_pushButton_3_clicked()
{
    QString str=ui->plainTextEdit->toPlainText();
    ui->plainTextEdit->setPlainText(ui->plainTextEdit_2->toPlainText());
    ui->plainTextEdit_2->setPlainText(str);
}

