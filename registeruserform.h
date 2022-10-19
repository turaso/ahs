#ifndef REGISTERUSERFORM_H
#define REGISTERUSERFORM_H

#include <QWidget>
#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QListWidgetItem>

#include "devicemodel.h"
#include "network.h"
#include "config.h"
#include "tokenizer.h"

namespace Ui {
class RegisterUserForm;
}

class RegisterUserForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterUserForm(QWidget *parent = nullptr);
    ~RegisterUserForm();

private slots:
    void on_registerButton_clicked();
    void on_exit();

signals:
    void windowClosing();

private:
    Ui::RegisterUserForm *ui;
    DeviceModel model_;
    QList<QListWidgetItem*> devices_;
    void closeEvent(QCloseEvent*) override;

};

#endif // REGISTERUSERFORM_H
