#include "planwindow.h"
#include "ui_planwindow.h"
#include "settingsform.h"
#include "catform.h"
#include "edittask.h"

#include <iostream>
#include <string>

#include <QPixmap>
#include <QIcon>
#include <iostream>

void planWindow::setPhotos()
{
    QPixmap heart_pix(":/img/heart.png");
    int w = ui->heart->width();
    int h = ui->heart->height();

    ui->heart->setPixmap(heart_pix.scaled(w, h, Qt::KeepAspectRatio));

    QPixmap profile_pix(":/img/cats/7.png");
    ui->profileImg->setPixmap(profile_pix.scaled(ui->profileImg->width(), ui->profileImg->height(), Qt::KeepAspectRatio));

   // QPixmap settings_pix(":/img/cats/7.png");
    ui->settingsButton->setIcon(QIcon(":/img/settings.png"));
    ui->settingsButton->setIconSize(QSize(45, 45));
}

// функция для загрузки данных о задачах
/*
void planWindow::uploadDataTasks()
{
    planWindow::tasks = new QString[planWindow::count_tasks_on_window];
	
	int rc = *client.get_all_tasks_by_userid(user_id);
	
	std::cout << "Result of take tasks = " << rc << "\n"; 

    tasks[0] = "Сделать технопарк";
    tasks[1] = "Почесать кота";
    tasks[2] = "Поспать";
}
*/

void planWindow::fillTasks()
{
	for(int i = 1, real = number_of_top_task; i <= count_tasks_on_window; i++, real++)
	{
		if real > tasks.size():
			real = 0;
			
		ui->numberLabel_1->setText(std::to_string(real + 1));
		ui->taskLabel_1->setText(planWindow::tasks[real]);
	}
}

planWindow::planWindow(std::string _user_id, InterClient *_client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::planWindow)
{
    ui->setupUi(this);
	
	client = _client;
	user_id = stoi(_user_id);
    user_name = _user_id;

	std::vector<std::string> _tasks = get_all_tasks_by_userid(user_id);
	for(int i = 0; i < _tasks.size(); i++)
		tasks.push_back(QString::fromStdString(_task[0]));
	
    setPhotos();
    fillTasks();
	
    ui->userNameLable->setText(planWindow::user_name);
}

planWindow::~planWindow()
{
    delete ui;
}

void planWindow::on_pushButton_2_clicked()
{
    QWidget::close();
}

void planWindow::on_settingsButton_clicked()
{
    settingsForm sets;
    sets.setModal(true);
    sets.exec();
}

void planWindow::on_see_cats_clicked()
{
    catform cats;
    cats.setModal(true);
    cats.exec();
}

void planWindow::on_create_clicked()
{
    QString text_of_task = ui->text_of_new_task->toPlainText();
	
	std::string:: text = text_of_task.toLatin1().data();
	
	if(text != "")
	{	
		tasks.push_front(QString::fromStdString(text));
		fillTasks();
		add_new_task(user_id, tasks.size() + 1, text);
	}
}

void planWindow::on_done_1_clicked()
{
    auto pos_of_elem = tasks.cbegin() + number_of_top_task;
    numbers2.erase(tasks.cbegin() + pos_of_elem, pos_of_elem);
    
    delete_task(user_id, task_id);
	
	fillTasks();
}


void planWindow::on_done_2_clicked()
{
    auto pos_of_elem = tasks.cbegin() + number_of_top_task + 1;
    numbers2.erase(tasks.cbegin() + pos_of_elem, pos_of_elem);

    delete_task(user_id, task_id);

    fillTasks();
}

void planWindow::on_done_3_clicked()
{
    auto pos_of_elem = tasks.cbegin() + number_of_top_task + 2;
    numbers2.erase(tasks.cbegin() + pos_of_elem, pos_of_elem);

    delete_task(user_id, task_id);

    fillTasks();
}

void planWindow::on_edit_1_clicked()
{
    edittask edit;
    edit.setModal(true);
    edit.exec();
}

void planWindow::on_edit_2_clicked()
{
    edittask edit;
    edit.setModal(true);
    edit.exec();
}

void planWindow::on_edit_3_clicked()
{
    edittask edit;
    edit.setModal(true);
    edit.exec();
}