#include "mainwindow.h"
#include <QApplication>
#include <easyform.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    easyForm w;
    w.show();

    return a.exec();
}
