#include "create_new_task.h"
#include "ui_create_new_task.h"
#include <iostream>

create_new_task::create_new_task(int _user_id, InterClient *_client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create_new_task)
{
    ui->setupUi(this);
	client = _client;
	user_id = _user_id;
}

create_new_task::~create_new_task()
{
    delete ui;
}

void create_new_task::on_create_clicked()
{
    QString text_of_task = ui->text_of_new_task->toPlainText();
	
	int rc = client->add_new_task(user_id, 0, text_of_task.toLatin1().data());
	
	std::cerr << "Result of add new task " << rc <<"\n";

    QWidget::close();
}
