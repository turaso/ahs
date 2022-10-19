#include "registerdeviceform.h"
#include "ui_registerdeviceform.h"

RegisterDeviceForm::RegisterDeviceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterDeviceForm)
{
    ui->setupUi(this);
    this->setFixedSize(Config::popupWidth, Config::popupHeight);
    QPoint p = this->mapToGlobal(
                QPoint(Config::width, Config::height)
                ) - QPoint(
                    this->size().width(),
                    this->size().height()
                ) - Config::popupDeviation;
    this->move(p);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->connect(this, &RegisterDeviceForm::windowClosing, this, &RegisterDeviceForm::on_exit);
}

RegisterDeviceForm::~RegisterDeviceForm()
{
    delete ui;
}

void RegisterDeviceForm::on_registerButton_clicked()
{
    QNetworkRequest req(Config::registerUrl);
    QJsonObject obj;
    obj.insert("username", ui->usernameEdit->text());
    obj.insert("password", ui->passwordEdit->text());
    auto reply = Network::instance().post(Tokenizer::tokenize(req), QJsonDocument(obj).toJson());
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QColor col;
    if (!reply->error() && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        col = Qt::green;
        QString qss = QString("background-color: %1").arg(col.name());
        ui->registerButton->setStyleSheet(qss);
        ui->statusLabel->setText("Status - OK");
        emit this->tableChanged();
    }
    else
    {
        col = Qt::red;
        QString qss = QString("background-color: %1").arg(col.name());
        ui->registerButton->setStyleSheet(qss);
        ui->statusLabel->setText("Status - ERROR");
    }
    reply->deleteLater();

}

void RegisterDeviceForm::closeEvent(QCloseEvent* event)
{
    emit windowClosing();
    event->accept();
}

void RegisterDeviceForm::on_exit()
{
    ui->usernameEdit->setText("");
    ui->passwordEdit->setText("");
    ui->statusLabel->setText("");
    ui->registerButton->setStyleSheet("");
    ui->usernameEdit->setFocus();
}
