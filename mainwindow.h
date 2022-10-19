#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSqlTableModel>
#include <QAuthenticator>
#include <QTimer>

#include "loginform.h"
#include "tokenizer.h"
#include "network.h"
#include "config.h"
#include "devicemodel.h"
#include "registerdeviceform.h"
#include "deviceform.h"
#include "registeruserform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();
    void on_tokenChanged();
    void on_getTableFinished(QNetworkReply*);
    void on_tableChanged();
    void on_tableView_doubleClicked(const QModelIndex& index);
    void on_updateTimer();
    void on_saveTimer();
    void on_regDeviceButton_clicked();
    void on_enableAllButton_clicked();
    void on_registerUserButton_clicked();
    void on_shutdownAllButton_clicked();
    void on_logoutButton_clicked();
    void on_updateButton_clicked();
    void on_exitButton_clicked();
    void on_saveButton_clicked();

signals:
    void tableChanged();

private:
    Ui::MainWindow *ui;
    LoginForm lf_;
    DeviceForm df_;
    DeviceModel model_;
    RegisterDeviceForm rd_;
    RegisterUserForm ru_;
    StatusDelegate combo3Delegate_;
    QTimer *updateTimer_ = new QTimer(this);
    int timeLeft_ = Config::updateInterval;
    QTimer *saveChangedTimer_  = new QTimer(this);

    void updateTableView();
    void enableAdminButtons();
    void enableUserButtons();
    void reset();
};
#endif // MAINWINDOW_H
