#include "planwindow.h"
#include "ui_planwindow.h"

planWindow::planWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::planWindow)
{
    ui->setupUi(this);

    QPixmap pix(":/img/heart.png");
    int w = ui->heart->width();
    int h = ui->heart->height();

    ui->heart->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
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
