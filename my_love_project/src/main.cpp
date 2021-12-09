#include "mainwindow.h"
#include "client.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style.css");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
	
	InterClient client;
	
    MainWindow w(client);
	
    w.show();
    return a.exec();
}
