#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>
#include <string>
#include <QAbstractTableModel>
#include <QList>
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QPair>
#include <QPainter>
#include <QJsonObject>

enum class DeviceModelColumns
{
    NAME_COLUMN = 0,
    STATUS_COLUMN = 1,
    MESSAGE_COLUMN = 2,
    SAVE_COLUMN = 3
};

class DeviceItem : public QStyledItemDelegate
{
public:
    std::string name;
    std::string status;
    std::string message;
    std::string programm;
    std::string save = "Save";
    DeviceItem(const DeviceItem& d)
    {
        this->name = d.name;
        this->status = d.status;
        this->message = d.message;
        this->programm = d.programm;
    }
    DeviceItem& operator=(const DeviceItem& d)
    {
        this->name = d.name;
        this->status = d.status;
        this->message = d.message;
        this->programm = d.programm;
        return *this;
    }
    DeviceItem(std::string name, std::string status, std::string message, std::string programm);
};

class Status
{
public:
    const static QString ACTIVE;
    const static QString DISABLED;
    const static QString INACTIVE;
private:
    Status() = delete;
    ~Status() = delete;
    Status(const Status&) = delete;
    Status& operator=(const Status&) = delete;
    Status(Status&&) = delete;
    Status& operator=(Status&&) = delete;
};

class StatusModel : public QAbstractListModel
{
    QList<QPair<QString, QString>> data_;
public:
    StatusModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QString getStatus(const int index) const;
};

class StatusDelegate : public QStyledItemDelegate
{
    // Q_DISABLE_COPY(Combo3Delegate)
    Q_OBJECT
public:
    StatusModel* statusModel = new StatusModel();
    int statusColumn = static_cast<int>(DeviceModelColumns::STATUS_COLUMN);
    StatusDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const override;
    ~StatusDelegate() override
    {
        delete statusModel;
    }
signals:
    void statusChanged(const int tableIndex, const int comboIndex) const;
};

class DeviceModel : public QAbstractTableModel
{
    int columns_ = 3;
    static QList<DeviceItem> devices_;
    static QList<QModelIndex> changedIndexes_;
public:
    DeviceModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void paint(const QModelIndex &index);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void setDevices(QList<DeviceItem> devices);
    void removeDevices();
    void changeDeviceStatus(const int index, const QString status);
    QJsonObject getJsonDeviceChange(const QModelIndex &index) const;
    bool isDeviceChanged(const int index) const;
    bool removeDeviceIfChanged(const int index);
    void removeChangedDevices();
    QString getDeviceName(const int index) const;
    QList<QString> getDevices() const;
    QList<QModelIndex> getChangedIndexes() const;
    QString getDeviceMessage(const int index) const;
    QString getDeviceProgramm(const int index) const;
};

#endif // DEVICEMODEL_H
