#ifndef EDITTASK_H
#define EDITTASK_H

#include <QDialog>
#include <iostream>

namespace Ui {
class edittask;
}

class edittask : public QDialog
{
    Q_OBJECT

public:
    explicit edittask(std::string *_text_of_task, QWidget *parent = nullptr);
    ~edittask();

private slots:
	void on_create_clicked();
	
private:
    Ui::edittask *ui;
	std::string *text_of_task;
};

#endif // EDITTASK_H
