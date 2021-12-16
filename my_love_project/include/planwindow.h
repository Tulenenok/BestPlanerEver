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
    explicit planWindow(std::string _user_id, InterClient *_client, QWidget* parent = nullptr);
    ~planWindow();
    
    InterClient *client;
    int user_id;

private slots:
    void on_pushButton_2_clicked();

    void on_settingsButton_clicked();
	
	void on_create_clicked();

private:
    Ui::planWindow* ui;

    QString user_name;
    QString* tasks;

    int count_tasks = 3;

    void setPhotos();
    void uploadDataTasks();
    void fillTasks();
};

#endif // PLANWINDOW_H
