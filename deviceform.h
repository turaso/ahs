#ifndef DEVICEFORM_H
#define DEVICEFORM_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QListWidgetItem>

#include "network.h"
#include "tokenizer.h"
#include "config.h"
#include "devicemodel.h"
#include "statuslistwidgetitem.h"

namespace Ui {
class DeviceForm;
}

class DeviceForm : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceForm(QWidget *parent = nullptr);
    ~DeviceForm();
    void setInfo();
    void setDeviceName(QString name);
    void setDeviceScript(QString script);

private slots:
    void on_saveButton_clicked();
    void on_removeButton_clicked();

signals:
    void tableChanged();

private:
    Ui::DeviceForm *ui;
    DeviceModel model_;
    QList<QListWidgetItem*> devices_;
    void clear();
    void setAllowedDevicesView(const QJsonArray& response, const QList<QString>& devices);
    void disableAllowedDevicesView();
    void disableButtons();
    void setLabelAllowed();
};

#endif // DEVICEFORM_H
