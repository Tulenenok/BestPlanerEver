#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style.css");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    MainWindow w;

    w.setFixedSize(16777215, 16777215);

    w.show();
    return a.exec();
}
