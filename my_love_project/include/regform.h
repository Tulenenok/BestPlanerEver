#ifndef REGFORM_H
#define REGFORM_H

#include <QDialog>
#include "client.h"

namespace Ui {
class regform;
}

class regform : public QDialog
{
    Q_OBJECT

public:
    explicit regform(InterClient* _client, QWidget *parent = nullptr);
    ~regform();

private slots:
    void on_pushButton_clicked();

private:
    Ui::regform *ui;
    InterClient* client;
};

#endif // REGFORM_H
