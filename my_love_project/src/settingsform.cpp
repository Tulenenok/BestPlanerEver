#include "settingsform.h"
#include "ui_settingsform.h"

settingsForm::settingsForm(QString  _user_name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsForm)
{
    ui->setupUi(this);

    QPixmap profile_pix(":/img/cats/7.png");
    ui->profileImg_2->setPixmap(profile_pix.scaled(ui->profileImg_2->width(), ui->profileImg_2->height(), Qt::KeepAspectRatio));

    // QPixmap cat_pix(":/img/cats/8.jpg");
    // ui->catImg->setPixmap(cat_pix.scaled(ui->catImg->width(), ui->catImg->height(), Qt::KeepAspectRatio));

    user_name = _user_name;
    ui->loginLabel->setText(settingsForm::user_name);
}

settingsForm::~settingsForm()
{
    delete ui;
}

void settingsForm::on_pushButton_clicked()
{
    QWidget::close();
}
