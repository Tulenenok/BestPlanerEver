#include "planwindow.h"
#include "ui_planwindow.h"
#include "settingsform.h"

#include <QPixmap>
#include <QIcon>

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

planWindow::planWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::planWindow)
{
    ui->setupUi(this);

    planWindow::setPhotos();

    planWindow::user_name = "Cat";                       // какое-то считываение данных о пользователе
    ui->userNameLable->setText(planWindow::user_name);
}

planWindow::~planWindow()
{
    delete ui;
}

// хорошо бы здесь сделать высплывающее окошко с вопросом
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
