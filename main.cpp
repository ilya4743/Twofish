#include "mainwindow.h"
#include "twofish.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Twofish twofish;
    Controller controller(&twofish);
    MainWindow w(&controller);
    w.show();
    return a.exec();
}
