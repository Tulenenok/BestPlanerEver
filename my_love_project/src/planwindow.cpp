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

#define NOT_VALUE_FOR_FIELD -1

static int is_not_empty_string(std::string str)
{
	for(int i = 0; i < str.size(); i++)
		if(i != " ")
			return 1;
		
	return 0;
}

void planWindow::delete_task_if_it_empty()
{
	for(int i = 0; i < tasks.size(); i++)
		if(is_not_empty_string(tasks[i]))
		{
			tasks.erase(pos_of_elem, pos_of_elem + 1);                      // удалили текст задачи
	i		id_tasks.erase(pos_of_elem, pos_of_elem + 1);                   // удалили id задачи
			i--;
		}
}

planWindow::planWindow(std::string _user_id, InterClient *_client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::planWindow)
{
    ui->setupUi(this);
	
	client = _client;
	user_id = stoi(_user_id);
    user_name = QString::fromStdString("Cat");

	tasks = client->get_all_tasks_by_userid(user_id);                                  // получили список всех тасок для пользователя
	for(int i = 0; i < tasks.size(); i++)                                              // заполнили все айдишники (нумерация с 0)
		id_tasks[i] = i;
	
	delete_task_if_it_empty();                                                         // удалили те задачи, которые пришли пустыми
	
	for(int i = 0; i < count_tasks_on_window; i++)                                     // заполнили как будто ничего показывать не нужно
		numbers_of_tasks.push_back(NOT_VALUE_FOR_FIELD);
	
	for(int i = 0, j = 0; i < tasks.size() && j < count_tasks_on_window; i++, j++)     // заполнили те номера, которые нужно будет вывести 
		numbers_of_taks[j] = i;
	
    setPhotos();                                                                       // установили все фото
    fillTasks();                                                                       // заполнили значения тасок
	
    ui->userNameLable->setText(planWindow::user_name);                  
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
		if(numbers_of_tasks[i] == NOT_VALUE_FOR_FIELD)                                             // пустые значения
		{
			num_vec[i]->setText(QString::fromStdString(std::to_string(" ")));
			task_vec[i]->setText(QString::fromStdString(std::to_string(" ")));
		} 
		else
		{
			num_vec[i]->setText(QString::fromStdString(std::to_string(numbers_of_tasks[i])));     // значение соотв номера
			task_vec[i]->setText(QString::fromStdString(tasks[numbers_of_tasks[i]]));             // значение соотв задачи
		}
}

void planWindow::showLastTasks()
{
	if(id_tasks.size() < numbers_of_tasks.size())
	{
		int i = 0;
		for(; i < id_tasks.size(); i++)
			numbers_of_tasks[i] = id_tasks[i];
		
		for(; i < numbers_of_tasks.size(); i++)
			numbers_of_tasks[i] = -1;
		
		return ;
		
	}
	
	for(int i = numbers_of_tasks.size(); i > 0; i--)
		numbers_of_tasks[i] = id_tasks[id_tasks.size() - i];
	
	fillTasks();
}

void planWindow::on_pushButton_2_clicked()
{
	delete []numbers_of_tasks;
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

void planWindow::clean_form()
{
	QTextCursor cur = ui->text_of_new_task->textCursor();
    cur.movePosition(QTextCursor::Start);
    cur.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cur.removeSelectedText();

    ui->text_of_new_task->insertPlainText("");
}

void planWindow::on_create_clicked()
{
    QString text_of_task = ui->text_of_new_task->toPlainText();
	std::string text = text_of_task.toLatin1().data();
	
	if(is_not_empty_string(text))
	{	
		tasks.push_back(QString::fromStdString(text));              // добавили задачу в конец списка задач
		if(id_tasks.size() == 0)
			id_tasks.push_back(0);
		else
			id_tasks.push_back(id_tasks[id_tasks.size() - 1] + 1);  // новый айди задачи следующий после уже существуещего (последнего)
		                                         
		client->add_new_task(user_id, ++tasks.size(), text);        // залили задачу в бд
		
		showLastTasks();                                            // обновили 
	}
	else
		QMessageBox::warning(this, "Error", "Вы не заполнили поле, задача не была создана");
	
	clean_form();                                                   // почистили форму
}

void planWindow::shift_numbers(int from_index)
{
	int i = from_index;
	for(; i < count_tasks_on_window - 1; i++)                                                                          // все до последней просто сдвинули
		numbers_of_tasks[i] = numbers_of_tasks[i + 1];
	
	// если это сработает, это будет чудом
	if(numbers_of_tasks[i - 1] == NOT_VALUE_FOR_FIELD || numbers_of_tasks[i] >= id_tasks[id_tasks.size() - 1])         // если следующая таска не существует
		numbers_of_taks[i] = NOT_VALUE_FOR_FIELD;
	else
		numbers_of_tasks[i] = id_tasks[numbers_of_tasks[i] + 1];
}

void planWindow::shift_numbers_down()
{
	int i = count_tasks_on_window - 1;
	for(; i > 0; i--)                                                                          // все до последней просто сдвинули
		numbers_of_tasks[i] = numbers_of_tasks[i - 1];
	
	// если это сработает, это будет чудом                           (дисклеймер : это точно не сработает)
	if(numbers_of_tasks[i - 1] == NOT_VALUE_FOR_FIELD || numbers_of_tasks[i] >= id_tasks[id_tasks.size() - 1])         // если следующая таска не существует
		numbers_of_taks[i] = NOT_VALUE_FOR_FIELD;
	else
		numbers_of_tasks[i] = id_tasks[numbers_of_tasks[i] + 1];
}

void planWindow::delete_task_by_index(int index)
{
	if(numbers_of_tasks[index] == NOT_VALUE_FOR_FIELD)
		return ;
	
	int num = numbers_of_tasks[index];                   // вытащили под каким айдишником лежит удаляемая задача
    auto pos_of_elem = tasks.cbegin() + num + index;     // нашли позицию этой задачи
	
    tasks.erase(pos_of_elem, pos_of_elem + 1);                      // удалили текст задачи
	id_tasks.erase(pos_of_elem, pos_of_elem + 1);                   // удалили id задачи
    
    client->delete_task(user_id, num);                   // удалили задачу с сервера
	
	shift_numbers(index);                                           // переделали индексы
	fillTasks();                                                    // перезаполнили 
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
	if(numbers_of_tasks[index] == -1)
	{
		QMessageBox::warning(this, "Error", "Это пустое поле, его нельзя редактировать");
		return ;
	}
	
	edittask edit;
    edit.setModal(&tasks[numbers_of_taks[index]], true);
    edit.exec();
	
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
    shift_numbers(0);
}

void planWindow::on_button_down_clicked()
{
    shift_numbers_down();
}