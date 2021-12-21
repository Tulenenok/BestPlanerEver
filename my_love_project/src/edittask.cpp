#include "edittask.h"
#include "ui_edittask.h"
#include <QMessageBox>

static int is_not_empty_string(std::string str)
{
	for(int i = 0; i < str.size(); i++)
		if(i != ' ')
			return 1;
		
	return 0;
}

edittask::edittask(std::string *_text_of_task, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edittask)
{
    ui->setupUi(this);
	text_of_task = _text_of_task;
	
	ui->textNow->setPlainText(QString::fromStdString(*text_of_task));
	ui->textNew->setPlainText(QString::fromStdString(*text_of_task));
}

edittask::~edittask()
{
    delete ui;
}

void edittask::on_create_clicked()
{
	QString _new_text = ui->textNew->toPlainText();
	std::string new_text = _new_text.toLatin1().data();
	
    if(is_not_empty_string(new_text))
	{
		*text_of_task = new_text;
		QWidget::close();
	}
	else
		QMessageBox::warning(this, "Error", "Вы не заполнили поле, задача не была изменена");
}
