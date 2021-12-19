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
	
<<<<<<< HEAD
	std::vector<std::string> rc = client.get_all_tasks_by_userid(user_id);
=======
	int rc = *client.get_all_tasks_by_userid(user_id);
>>>>>>> cd526b79c7fe089d6979fe8c961f3c06e59f7793
	
	std::cout << "Result of take tasks = " << "\n";
	for (auto&task : rc) {
		std::cout << task << std::endl;
	}

    tasks[0] = "Сделать технопарк";
    tasks[1] = "Почесать кота";
    tasks[2] = "Поспать";
}
*/

void planWindow::fillTasks()
{

    std::vector<QLabel*> num_vec;
    num_vec.push_back(ui->numberLabel_1);
    num_vec.push_back(ui->numberLabel_2);
    num_vec.push_back(ui->numberLabel_3);

    std::vector<QLabel*> task_vec;
    task_vec.push_back(ui->taskLabel_1);
    task_vec.push_back(ui->taskLabel_2);
    task_vec.push_back(ui->taskLabel_3);

	for(int i = 1, real = number_of_top_task; i <= count_tasks_on_window; i++, real++)
	{
		if (real > tasks.size()) {
			real = 0;
        }
			
		num_vec[i-1]->setText(QString::fromStdString(std::to_string(real + 1)));
		task_vec[i-1]->setText(planWindow::tasks[real]);
	}
}

planWindow::planWindow(std::string _user_id, InterClient *_client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::planWindow)
{
    ui->setupUi(this);
	
	client = _client;
	user_id = stoi(_user_id);
    user_name = QString::fromStdString(_user_id);//user_id != user_name

	std::vector<std::string> _tasks = client->get_all_tasks_by_userid(user_id);
	for(int i = 0; i < _tasks.size(); i++)
		tasks.push_back(QString::fromStdString(_tasks[i]));
	
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
	
	std::string text = text_of_task.toLatin1().data();
	
	if(text != "")
	{	
		tasks.push_back(QString::fromStdString(text));
		fillTasks();
		client->add_new_task(user_id, tasks.size() + 1, text);
	}
}

void planWindow::on_done_1_clicked()
{
    auto pos_of_elem = tasks.cbegin() + number_of_top_task;
    tasks.erase(tasks.cbegin() + number_of_top_task, tasks.cbegin() + number_of_top_task + 1);
    
    client->delete_task(user_id, 0);
	
	fillTasks();
}


void planWindow::on_done_2_clicked()
{
    auto pos_of_elem = tasks.cbegin() + number_of_top_task + 1;
    tasks.erase(tasks.cbegin() + 1 + number_of_top_task, tasks.cbegin() + number_of_top_task + 2);

    client->delete_task(user_id, 1);

    fillTasks();
}

void planWindow::on_done_3_clicked()
{
    auto pos_of_elem = tasks.cbegin() + number_of_top_task + 2;
    tasks.erase(tasks.cbegin()  + 2 + number_of_top_task, tasks.cbegin() + number_of_top_task + 3);

    client->delete_task(user_id, 2);

    fillTasks();
}

void planWindow::on_edit_1_clicked()
{
    // edittask edit;
    // edit.setModal(true);
    // edit.exec();
}

void planWindow::on_edit_2_clicked()
{
    // edittask edit;
    // edit.setModal(true);
    // edit.exec();
}

void planWindow::on_edit_3_clicked()
{
    // edittask edit;
    // edit.setModal(true);
    // edit.exec();
}