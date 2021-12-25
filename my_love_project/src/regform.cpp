#include "regform.h"
#include "ui_regform.h"
#include <QString>
#include <QFile>
#include <QMessageBox>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

regform::regform(InterClient* _client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regform)
{
    client = _client;
    ui->setupUi(this);
}

regform::~regform()
{
    delete ui;
}

// функция, которая обрабатывает введенные значения
void regform::on_pushButton_clicked()
{
    QString name = ui->name->text();
    QString mail = ui->mail->text();
    QString password = ui->password->text();
    QString password_again = ui->password_again->text();

    if(QString::compare(password, password_again, Qt::CaseInsensitive) == 0) {
        client->registration(name.toLatin1().data(), password.toLatin1().data(), mail.toLatin1().data());
    }

    QWidget::close();
}
