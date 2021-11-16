#ifndef PLANWINDOW_H
#define PLANWINDOW_H

#include <QDialog>

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

private:
    Ui::planWindow *ui;
};

#endif // PLANWINDOW_H
