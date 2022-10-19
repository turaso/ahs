#include "deviceform.h"
#include "ui_deviceform.h"

DeviceForm::DeviceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceForm)
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
    ui->nameLabel->setText("");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
}

DeviceForm::~DeviceForm()
{
    delete ui;
}

void DeviceForm::clear()
{
    devices_.clear();
    ui->listWidget->clear();
}

void DeviceForm::setAllowedDevicesView(const QJsonArray& response, const QList<QString>& devices)
{
    ui->labelAllowed->setEnabled(true);
    ui->listWidget->setEnabled(true);
    ui->saveButton->setEnabled(true);
    if (Tokenizer::role == "ADMIN")
    {
        ui->removeButton->setEnabled(true);
    }
    else
    {
        ui->removeButton->setEnabled(false);
    }
    ui->labelCommand->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    for (QString dev : devices)
    {
        StatusListWidgetItem* item = new StatusListWidgetItem(dev, response.contains(dev), Tokenizer::role == "ADMIN");
        devices_.append(item);
        ui->listWidget->addItem(item);
    }
    ui->saveButton->setStyleSheet("");
    ui->listWidget->update();
}

void DeviceForm::disableAllowedDevicesView()
{
    ui->labelAllowed->setEnabled(false);
    ui->listWidget->setEnabled(false);
    ui->labelCommand->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->removeButton->setEnabled(false);
    ui->saveButton->setStyleSheet("");
}

void DeviceForm::disableButtons()
{
    QColor col;
    col = Qt::gray;
    QString qss = QString("background-color: %1").arg(col.name());
    ui->saveButton->setStyleSheet(qss);
    ui->saveButton->setEnabled(false);
    ui->removeButton->setEnabled(false);
}

void DeviceForm::setLabelAllowed()
{
    if (Tokenizer::role == "ADMIN")
    {
        ui->labelAllowed->setEnabled(true);
    }
    else
    {
        ui->labelAllowed->setEnabled(false);
    }
}

void DeviceForm::setInfo()
{
    QNetworkRequest req(Config::deviceUrl + ui->nameLabel->text());
    auto reply = Network::instance().get(Tokenizer::tokenize(req));
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    this->clear();
    if (!reply->error() && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        QByteArray bytes = reply->readAll();
        reply->deleteLater();
        QJsonDocument doc = QJsonDocument::fromJson(bytes);
        QJsonArray arr = doc.object().value("devices").toArray();
        auto devices = model_.getDevices();
        devices.removeAll(ui->nameLabel->text());
        if (!devices.empty())
        {
            this->setAllowedDevicesView(arr, devices);
        }
        else
        {
            this->disableAllowedDevicesView();
        }
    }
    else
    {
        this->disableButtons();
    }
    this->setLabelAllowed();
}

void DeviceForm::setDeviceName(QString name)
{
    ui->nameLabel->setText(name);
}

void DeviceForm::setDeviceScript(QString script)
{
    ui->lineEdit->setText(script);
}

void DeviceForm::on_saveButton_clicked()
{
    QNetworkRequest req(Config::setDeviceConfigUrl);
    QJsonObject obj;
    obj.insert("username", ui->nameLabel->text());
    obj.insert("script", ui->lineEdit->text());
    QJsonArray arr;
    for (QListWidgetItem *i : devices_)
    {
        if (i->checkState() == Qt::Checked)
        {
            arr.append(i->text());
        }
    }
    obj.insert("devices", arr);
    auto reply = Network::instance().post(Tokenizer::tokenize(req), QJsonDocument(obj).toJson());
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QColor col;
    if (!reply->error() && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        emit this->tableChanged();
        col = Qt::green;
        QString qss = QString("background-color: %1").arg(col.name());
        ui->saveButton->setStyleSheet(qss);
    }
    else
    {
        col = Qt::red;
        QString qss = QString("background-color: %1").arg(col.name());
        ui->saveButton->setStyleSheet(qss);
    }
}

void DeviceForm::on_removeButton_clicked()
{
    QNetworkRequest req(Config::removeUrl);
    QJsonObject obj;
    obj.insert("devicename", ui->nameLabel->text());
    auto reply = Network::instance().post(Tokenizer::tokenize(req), QJsonDocument(obj).toJson());
    QEventLoop loop;
    connect(&Network::instance(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QColor col;
    if (!reply->error() && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        emit this->tableChanged();
        this->close();
    }
    else
    {
        col = Qt::red;
        QString qss = QString("background-color: %1").arg(col.name());
        ui->removeButton->setStyleSheet(qss);
    }
    reply->deleteLater();
}
