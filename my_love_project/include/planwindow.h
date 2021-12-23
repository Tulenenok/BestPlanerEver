#ifndef PLANWINDOW_H
#define PLANWINDOW_H

#include <QDialog>
#include <QString>
#include <iostream>
#include <string>
#include "client.h"

namespace Ui
{
class planWindow;
}

class planWindow : public QDialog
{
    Q_OBJECT

public:
    explicit planWindow(std::string _user_id, std::string _user_name, InterClient *_client, QWidget* parent = nullptr);
    ~planWindow();
    
    InterClient *client;
    int user_id;

private slots:
    void on_pushButton_2_clicked();

    void on_settingsButton_clicked();
	
	void on_create_clicked();
	
	void on_see_cats_clicked();
	
	void on_done_1_clicked();
	void on_done_2_clicked();
	void on_done_3_clicked();
	
	void on_edit_1_clicked();
    void on_edit_2_clicked();
    void on_edit_3_clicked();
	
	void on_button_up_clicked();
	void on_button_down_clicked();

private:
    Ui::planWindow* ui;

    QString user_name;
	std::vector<std::string> tasks;
	std::vector<int> id_tasks;

    int count_tasks_on_window = 3;
	int number_of_top_task = 0;
	std::vector<int> numbers_of_tasks;

    void setPhotos();
    void uploadDataTasks();
    void fillTasks();
	void delete_task_by_index(int index);
	void delete_task_if_it_empty();
	void shift_up();
	void shift_down();
	void showTasksNearNumber(int number);
	void clean_form();
	void edit_task_by_index(int index);
};

#endif // PLANWINDOW_H
