#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "catform.h"
#include "regform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// форма регистрации
void MainWindow::on_pushButton_2_clicked()
{
    regform reg;
    reg.setModal(true);
    reg.exec();
}

// форма с котиками
void MainWindow::on_pushButton_3_clicked()
{
    catform cats;
    cats.setModal(true);
    cats.exec();
}

void MainWindow::on_pushButton_clicked()
{
    catform cats;
    cats.setModal(true);
    cats.exec();
}
