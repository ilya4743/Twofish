#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "controller.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    Controller *controller;
public:
    MainWindow(Controller *controller, QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_action_5_triggered();
    void on_comboBox_currentIndexChanged(int index);
};
#endif // MAINWINDOW_H
