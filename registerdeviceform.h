#ifndef REGISTERDEVICEFORM_H
#define REGISTERDEVICEFORM_H

#include <QWidget>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCloseEvent>

#include "network.h"
#include "config.h"
#include "tokenizer.h"

namespace Ui {
class RegisterDeviceForm;
}

class RegisterDeviceForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterDeviceForm(QWidget *parent = nullptr);
    ~RegisterDeviceForm();

private slots:
    void on_registerButton_clicked();
    void on_exit();

signals:
    void tableChanged();
    void windowClosing();

private:
    Ui::RegisterDeviceForm *ui;
    void closeEvent(QCloseEvent*) override;
};

#endif // REGISTERDEVICEFORM_H
