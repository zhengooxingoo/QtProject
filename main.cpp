#include "mainwindow.h"
#include <QApplication>
#include<QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setAttribute(Qt::WA_QuitOnClose,true);
    w.show();
    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    return a.exec();
}
