#ifndef PLANWINDOW_H
#define PLANWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class planWindow;
}

class planWindow : public QDialog
{
    Q_OBJECT

public:
    explicit planWindow(QWidget *parent = nullptr);
    ~planWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_settingsButton_clicked();

private:
    Ui::planWindow *ui;

    QString user_name;

    void setPhotos();
};

#endif // PLANWINDOW_H
