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
#include <QMessageBox>
#include <QTextCursor>
#include <QApplication>

#define NOT_VALUE_FOR_FIELD -1

#define COUNT_TASKS_IN_SYSTEM 7

static int is_empty_task(std::string str)
{
	return str == "empty task";
}

static int is_empty_string(std::string str)
{
	for(int i = 0; i < str.size(); i++)
		if(str[i] != ' ')
			return 0;
		
	return 1;
}

void planWindow::delete_task_if_it_empty()
{
	for(int i = 0; i < tasks.size(); i++)
		if(is_empty_task(tasks[i]))
		{
			tasks.erase(tasks.cbegin() + i, tasks.cbegin() + i + 1);                     
			id_tasks.erase(id_tasks.cbegin() + i, id_tasks.cbegin() + i + 1);                   
			i--;
		}
}

planWindow::planWindow(std::string _user_id, std::string _user_name, InterClient *_client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::planWindow)
{
    ui->setupUi(this);
	
	client = _client;
	user_id = stoi(_user_id);
    user_name = QString::fromStdString(_user_name);

	tasks = client->get_all_tasks_by_userid(user_id);                                  // получили список всех тасок для пользователя

	for(int i = 0; i < tasks.size(); i++)                                              // заполнили все айдишники (нумерация с 0)
		id_tasks.push_back(i);
	
	for(int i = 0; i < count_tasks_on_window; i++)                                     // изначально будем показывать первые три номера
		numbers_of_tasks.push_back(i);

    setPhotos();                                                                       // установили все фото
    fillTasks();                                                                       // заполнили значения тасок
	
    ui->userNameLable->setText(planWindow::user_name); 

	ui->number_of_new_task->setPlaceholderText("number (1-7)");    
	ui->text_of_new_task->setPlaceholderText("text of new task");     
	//ui->quote->setStyleSheet("QLabel {margin-top: 5px;}")     
}

planWindow::~planWindow()
{
    delete ui;
}

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
	
	for(int i = 0; i < count_tasks_on_window; i++)          
	{
		num_vec[i]->setText(QString::fromStdString(std::to_string(numbers_of_tasks[i] + 1)));     // значение соотв номера
		task_vec[i]->setText(QString::fromStdString(tasks[numbers_of_tasks[i]]));                 // значение соотв задачи

		if(is_empty_task(tasks[numbers_of_tasks[i]]))                                           // пустые значения
		{
			num_vec[i]->setStyleSheet("QLabel {border: 2px solid rgb(128, 35, 243); border-radius: 5px; color:grey; font-size: 15px; padding-top: 7px; padding-left: 9px;}");
			task_vec[i]->setStyleSheet("QLabel {border: 2px solid rgb(128, 35, 243); border-radius: 5px; color:grey; font-size: 15px; padding-top: 5px;}");
		} 
		else
		{
			num_vec[i]->setStyleSheet("QLabel {border: 2px solid rgb(128, 35, 243); border-radius: 5px; color:white; font-size: 15px; padding-top: 7px; padding-left: 9px;}");
			task_vec[i]->setStyleSheet("QLabel {border: 2px solid rgb(128, 35, 243); border-radius: 5px; color:white; font-size: 15px; padding-top: 5px;}");
		}
	} 
}

void planWindow::showTasksNearNumber(int number)
{
	if(number <= 0 || number > COUNT_TASKS_IN_SYSTEM)
	{
		std::cerr << "Invalid number in showTasksNearNumber";
		return ;
	}

	number = number - 1;

	if(number < count_tasks_on_window)
		for(int i = 0; i < count_tasks_on_window; i++)
			numbers_of_tasks[i] = i;
	
	else if(number >= COUNT_TASKS_IN_SYSTEM - count_tasks_on_window)
		for(int i = COUNT_TASKS_IN_SYSTEM - count_tasks_on_window; i < COUNT_TASKS_IN_SYSTEM; i++)
			numbers_of_tasks[i] = i;

	else
		for(int i = 0; i < count_tasks_on_window; i++)
			numbers_of_tasks[i] = i + number;

	fillTasks();
}

void planWindow::on_pushButton_2_clicked()
{
    QWidget::close();
}

void planWindow::on_settingsButton_clicked()
{
    settingsForm sets(user_name);
    sets.setModal(true);
    sets.exec();
}

void planWindow::on_see_cats_clicked()
{
    catform cats;
    cats.setModal(true);
    cats.exec();
}

void planWindow::clean_form()
{
	QTextCursor cur = ui->text_of_new_task->textCursor();
    cur.movePosition(QTextCursor::Start);
    cur.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cur.removeSelectedText();

    ui->text_of_new_task->insertPlainText("");
	ui->number_of_new_task->setText("");
}

int number_of_task_from_str_to_int(std::string x)
{
	for(int i = 1; i <= COUNT_TASKS_IN_SYSTEM; i++)
		if(x == std::to_string(i))
			return i;

	return -1;	
}

void planWindow::on_create_clicked()
{
    QString text_of_task = ui->text_of_new_task->toPlainText();
	std::string text = text_of_task.toLatin1().data();

	QString num_of_task = ui->number_of_new_task->text();
	int number = number_of_task_from_str_to_int(num_of_task.toLatin1().data());

	if(number == -1)
	{
		QMessageBox::warning(this, "Error", "Номер задачи должен быть целым числом от 1 до 7");
		return ;
	}


	std::cerr << text;
	
	if(!is_empty_string(text))
	{
		tasks[number - 1] = text; 
		showTasksNearNumber(number);

		if(is_empty_task(tasks[number]))                       
			client->add_new_task(user_id, number, text);        // залили задачу в бд
		else
		{
			//QMessageBox::warning(this, "Error", "Задача была перезаписана");
			client->alter_task(user_id, number, text);                   
		}
		
	}
	else
		QMessageBox::warning(this, "Error", "Вы не заполнили поле, задача не была создана");
	
	clean_form();                                                   // почистили форму
}

void planWindow::shift_up() 
{
	for(int i = 0; i < count_tasks_on_window; i++)                                                                          
		numbers_of_tasks[i] =  (numbers_of_tasks[i] + 1) % COUNT_TASKS_IN_SYSTEM;
}

void planWindow::shift_down()
{
	for(int i = 0; i < count_tasks_on_window; i++)                                                                          
		if(numbers_of_tasks[i] != 0)
			numbers_of_tasks[i]--;
		else
			numbers_of_tasks[i] = COUNT_TASKS_IN_SYSTEM - 1;
}

void planWindow::delete_task_by_index(int index)
{
	int num = numbers_of_tasks[index];                   // вытащили под каким айдишником лежит удаляемая задача

	if(is_empty_task(tasks[num]))
	{
		QMessageBox::warning(this, "Error", "Задача уже выполнена");
		return ;
	}

	tasks[num] = "empty task";
    client->delete_task(user_id, num + 1);                   
	fillTasks();                                                    
}

void planWindow::on_done_1_clicked()
{
	delete_task_by_index(0);
}

void planWindow::on_done_2_clicked()
{
    delete_task_by_index(1);
}

void planWindow::on_done_3_clicked()
{
    delete_task_by_index(2);
}

void planWindow::edit_task_by_index(int index)
{
	edittask edit(&tasks[numbers_of_tasks[index]]);
    edit.setModal(true);
    edit.exec();
	
	client->alter_task(user_id, numbers_of_tasks[index] + 1, tasks[numbers_of_tasks[index]]);      
	fillTasks();
}

void planWindow::on_edit_1_clicked()
{
    edit_task_by_index(0);
}

void planWindow::on_edit_2_clicked()
{
    edit_task_by_index(1);
}

void planWindow::on_edit_3_clicked()
{
    edit_task_by_index(2);
}

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

void planWindow::on_button_up_clicked()
{
    shift_down();
    //shift_up();
    fillTasks();
}

void planWindow::on_button_down_clicked()
{
    //shift_down();
    shift_up();
    fillTasks();
}
