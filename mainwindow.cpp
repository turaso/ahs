#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(Config::width, Config::height);
    this->showFullScreen();
    ui->tableView->setModel(&model_);
    this->connect(&lf_.tokenizer, &Tokenizer::tokenChanged, this, &MainWindow::on_tokenChanged);
    this->connect(this, &MainWindow::tableChanged, this, &MainWindow::on_tableChanged);
    this->connect(&rd_, &RegisterDeviceForm::tableChanged, this, &MainWindow::on_tableChanged);
    this->connect(ui->tableView, &QTableView::doubleClicked, this, &MainWindow::on_tableView_doubleClicked);
    this->connect(&df_, &DeviceForm::tableChanged, this, &MainWindow::on_tableChanged);
    this->connect(updateTimer_, &QTimer::timeout, this, &MainWindow::on_updateTimer);
    this->connect(saveChangedTimer_, &QTimer::timeout, this, &MainWindow::on_saveTimer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_updateTimer()
{
    const static int zero = 0;
    if (zero < timeLeft_)
    {
        timeLeft_--;
    }
    else
    {
        timeLeft_ = Config::updateInterval;
        this->on_updateButton_clicked();
    }
    ui->updateButton->setText("Update " + QString::fromStdString(std::to_string(timeLeft_)) + "s");
}

void MainWindow::on_saveTimer()
{
    saveChangedTimer_->stop();
    ui->saveButton->setStyleSheet("");
}

void MainWindow::on_loginButton_clicked()
{
    lf_.show();
}

void MainWindow::on_getTableFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QList<DeviceItem> devices;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        reply->deleteLater();
        QJsonObject obj = doc.object();
        QJsonValue val = obj["devices"];
        QJsonArray arr = val.toArray();
        foreach (const QJsonValue & v, arr)
        {
            std::string name = v.toObject().value("username").toString().toStdString();
            std::string status = v.toObject().value("statuses").toObject().value("status").toString().toStdString();
            std::string message = v.toObject().value("info").toString().toStdString();
            std::string programm = v.toObject().value("program").toObject().value("script").toString().toStdString();
            DeviceItem dev(name, status, message, programm);
            devices.append(dev);
        }
        model_.setDevices(devices);
        ui->tableView->setItemDelegateForColumn(static_cast<int>(DeviceModelColumns::STATUS_COLUMN), &combo3Delegate_);
        ui->tableView->update();
        ui->tableView->horizontalHeader()->setSectionResizeMode(static_cast<int>(DeviceModelColumns::NAME_COLUMN), QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setSectionResizeMode(static_cast<int>(DeviceModelColumns::STATUS_COLUMN), QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setSectionResizeMode(static_cast<int>(DeviceModelColumns::MESSAGE_COLUMN), QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setVisible(true);
        ui->updateButton->setEnabled(true);
        timeLeft_ = Config::updateInterval;
        updateTimer_->start(1000);
        ui->updateButton->setStyleSheet("");
        ui->updateButton->setText("Update " + QString::fromStdString(std::to_string(Config::updateInterval)) + "s");
    }
    else
    {
        QString qss = QString("");
        ui->updateButton->setStyleSheet(qss);
    }
}

void MainWindow::updateTableView()
{
    QNetworkRequest req(Config::homeUrl);
    auto reply = Network::instance().get(Tokenizer::tokenize(req));
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    this->on_getTableFinished(reply);
}

void MainWindow::enableUserButtons()
{
    ui->enableAllButton->setEnabled(true);
    ui->shutdownAllButton->setEnabled(true);
    ui->logoutButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
}

void MainWindow::enableAdminButtons()
{
    ui->regDeviceButton->setEnabled(true);
    ui->registerUserButton->setEnabled(true);
}

void MainWindow::on_tokenChanged()
{
    this->reset();
    if (Tokenizer::role == "USER" || Tokenizer::role == "ADMIN")
    {
        this->updateTableView();
        this->enableUserButtons();
    }
    if (Tokenizer::role == "ADMIN")
    {
        this->enableAdminButtons();
    }
    QColor col = Qt::cyan;
    QString qss = QString("background-color: %1").arg(col.name());
    ui->loginButton->setStyleSheet(qss);
}

void MainWindow::on_tableChanged()
{
    this->updateTableView();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex& index)
{
    if (index.column() == static_cast<int>(DeviceModelColumns::NAME_COLUMN))
    {
        df_.setDeviceName(model_.getDeviceName(index.row()));
        df_.setInfo();
        df_.setDeviceScript(model_.getDeviceProgramm(index.row()));
        df_.show();
    }
}

void MainWindow::on_regDeviceButton_clicked()
{
    rd_.show();
}

void MainWindow::on_enableAllButton_clicked()
{
    QNetworkRequest req(Config::enableAllUrl);
    auto reply = Network::instance().post(Tokenizer::tokenize(req), QJsonDocument().toJson());
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (!reply->error() && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        emit this->tableChanged();
    }
    else
    {
        // TODO
    }
    reply->deleteLater();
}


void MainWindow::on_shutdownAllButton_clicked()
{
    QNetworkRequest req(Config::shutdownAllUrl);
    auto reply = Network::instance().post(Tokenizer::tokenize(req), QJsonDocument().toJson());
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (!reply->error() && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        emit this->tableChanged();
    }
    else
    {
        // TODO
    }
    reply->deleteLater();
}

void MainWindow::on_registerUserButton_clicked()
{
    ru_.show();
}

void MainWindow::reset()
{
    model_.removeDevices();
    ui->tableView->horizontalHeader()->setVisible(false);
    emit this->tableChanged();
    ui->logoutButton->setEnabled(false);
    ui->updateButton->setEnabled(false);
    updateTimer_->stop();
    ui->updateButton->setText("Update");
    ui->loginButton->setStyleSheet("");
    ui->shutdownAllButton->setEnabled(false);
    ui->enableAllButton->setEnabled(false);
    ui->regDeviceButton->setEnabled(false);
    ui->registerUserButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    lf_.resetStatus();
}

void MainWindow::on_logoutButton_clicked()
{
    QUrl url(Config::logoutUrl);
    QNetworkRequest req(url);
    auto reply = Network::instance().post(Tokenizer::tokenize(req), QJsonDocument().toJson());
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (!reply->error()) // status is not equal "200"
    {
        Tokenizer::resetToken();
        this->reset();
    }
    else
    {
        // TODO
    }
    reply->deleteLater();
}

void MainWindow::on_updateButton_clicked()
{
    emit this->tableChanged();
}

void MainWindow::on_exitButton_clicked()
{
    lf_.close();
    df_.close();
    rd_.close();
    ru_.close();
    this->close();
}

void MainWindow::on_saveButton_clicked()
{
    QNetworkRequest req(Config::changeDevicesStatusUrl);
    QJsonArray arr;
    for (QModelIndex i : model_.getChangedIndexes())
    {
        arr.push_back(model_.getJsonDeviceChange(i));
    }
    QJsonObject obj;
    obj.insert("changed", arr);
    auto reply = Network::instance().post(Tokenizer::tokenize(req), QJsonDocument(obj).toJson());
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QColor col;
    const int time = 3000;
    if (!reply->error() && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        model_.removeChangedDevices();
        col = Qt::green;
        QString qss = QString("background-color: %1").arg(col.name());
        ui->saveButton->setStyleSheet(qss);
        saveChangedTimer_->start(time);
    }
    else
    {
        col = Qt::red;
        QString qss = QString("background-color: %1").arg(col.name());
        ui->saveButton->setStyleSheet(qss);
        saveChangedTimer_->start(time);
    }
    reply->deleteLater();
}
