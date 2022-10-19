#ifndef LOGINFORM_H
#define LOGINFORM_H

#include "config.h"
#include "tokenizer.h"

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <network.h>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();
    Tokenizer tokenizer;
    void resetStatus();

private slots:
    void on_request_finished(QNetworkReply *reply);
    void on_okButtin_clicked();
//    void on_cancelButton_clicked();

private:
    Ui::LoginForm *ui;;
};

#endif // LOGINFORM_H
