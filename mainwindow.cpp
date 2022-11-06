#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "myexception.h"

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

void MainWindow::on_pushButton_clicked()
{
    try
    {
        string str1=ui->lineEdit->text().toStdString();
        string str2=ui->plainTextEdit->toPlainText().toStdString();
        bool isKeyHex=ui->radioButton_3->isChecked();
        bool isPTHex=ui->radioButton_2->isChecked();
        string out=controller->encrypt(std::move(str1),std::move(str2), isKeyHex, isPTHex);
        ui->plainTextEdit_2->setPlainText(QString::fromStdString(out));
    }
    catch (myexception& e)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка",e.what());
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    try
    {
        string str1=ui->lineEdit->text().toStdString();
        string str2=ui->plainTextEdit->toPlainText().toStdString();
        bool isKeyHex=ui->radioButton_3->isChecked();
        bool isPTHex=ui->radioButton_2->isChecked();
        string out=controller->decrypt(std::move(str1),std::move(str2), isKeyHex, isPTHex);
        ui->plainTextEdit_2->setPlainText(QString::fromStdString(out));
    }
    catch (myexception& e)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка",e.what());
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    QString str=ui->plainTextEdit->toPlainText();
    ui->plainTextEdit->setPlainText(ui->plainTextEdit_2->toPlainText());
    ui->plainTextEdit_2->setPlainText(str);
}


void MainWindow::on_action_triggered()
{
    QCoreApplication:: quit();
}

void MainWindow::on_action_2_triggered()
{
    try
    {
        QString base = QDir::currentPath();
        QString path = QString( "%1/result1.txt" ).arg( base );
        int idx = 1;
        while ( QFile::exists( path ) ) {
            path = QString( "%1/result%2.txt" ).arg( base ).arg( idx++ );
        }
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Сохранить результат работы программы"), path,
            tr("result (*.txt);;All Files (*.txt)"));
        string out=ui->plainTextEdit_2->toPlainText().toStdString();
        controller->printResults(std::move(fileName.toStdString()),std::move(out));
    }
    catch(myexception& e)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка",e.what());
    }
}


void MainWindow::on_action_3_triggered()
{
    try
    {
        QString fileName1 = QFileDialog::getOpenFileName(this,
            tr("Выберите файл с ключом"), "",tr("All Files (*.txt)"));
        string key=controller->loadKey(std::move(fileName1.toStdString()));
        ui->lineEdit->setText(QString::fromStdString(key));

        QString fileName2 = QFileDialog::getOpenFileName(this,
            tr("Выберите файл с входными данными"), "",tr("All Files (*.txt)"));
        string text=controller->loadInputText(std::move(fileName1.toStdString()));
        ui->plainTextEdit->setPlainText(QString::fromStdString(text));
    }
    catch(myexception& e)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка",e.what());
    }
}


void MainWindow::on_action_5_triggered()
{
    QMessageBox messageBox;
    messageBox.information(0,"О программе","\nКурсовой проект студента К3-32М\nМФ МГТУ им Н. Э. Баумана\nСоломатина Ильи\n"
    "ilasolomatin29@gmail.com\nРеализация алгоритма шифрования Twofish\n\nTwofish — симметричный алгоритм блочного шифрования с размером блока "
    "128 бит и длиной ключа до 256 бит. Число раундов 16. Разработан группой специалистов во главе с Брюсом Шнайером. "
    "Являлся одним из пяти финалистов второго этапа конкурса AES. Алгоритм разработан на основе алгоритмов Blowfish, SAFER и SQUARE. ");
}

