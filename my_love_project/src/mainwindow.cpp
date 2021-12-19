#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "catform.h"
#include "regform.h"
#include "planwindow.h"

MainWindow::MainWindow(InterClient *_client, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = _client;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// основной вход
void MainWindow::on_pushButton_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();

	int rc = client->login(login.toLatin1().data(),  password.toLatin1().data());
	std::cout << "Result of check" << rc;
	
	if(rc != -1)
	{
		hide();
		planWindow plan(std::to_string(rc), client);
		plan.setModal(true);
		plan.exec();
	}
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
