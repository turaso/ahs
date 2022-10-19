#include "devicemodel.h"

DeviceItem::DeviceItem(std::string name, std::string status, std::string message, std::string programm)
{
    this->name = name;
    this->status = status;
    this->message = message;
    this->programm = programm;
}

DeviceModel::DeviceModel(QObject *parent) : QAbstractTableModel(parent)
{
    this->setHeaderData(static_cast<int>(DeviceModelColumns::NAME_COLUMN), Qt::Horizontal, QObject::tr("Name"));
    this->setHeaderData(static_cast<int>(DeviceModelColumns::STATUS_COLUMN), Qt::Horizontal, QObject::tr("Status"));
    this->setHeaderData(static_cast<int>(DeviceModelColumns::MESSAGE_COLUMN), Qt::Horizontal, QObject::tr("Info"));
}

QList<DeviceItem> DeviceModel::devices_ = QList<DeviceItem>();
QList<QModelIndex> DeviceModel::changedIndexes_ = QList<QModelIndex>();

int DeviceModel::rowCount(const QModelIndex &) const
{
    return devices_.size();
}

int DeviceModel::columnCount(const QModelIndex &) const
{
    return this->columns_;
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case static_cast<int>(DeviceModelColumns::NAME_COLUMN):
            return QString::fromStdString(devices_.at(index.row()).name);
        case static_cast<int>(DeviceModelColumns::STATUS_COLUMN):
            return QString::fromStdString(devices_.at(index.row()).status);
        case static_cast<int>(DeviceModelColumns::MESSAGE_COLUMN):
            return QString::fromStdString(devices_.at(index.row()).message);
        default:
            return "Unknown";
        }
    }
    else if (role == Qt::BackgroundRole)
    {
        if (index.column() == static_cast<int>(DeviceModelColumns::STATUS_COLUMN))
        {
            if (devices_.at(index.row()).status == Status::ACTIVE.toStdString())
            {
                return QBrush(Qt::green);
            }
            else if (devices_.at(index.row()).status == Status::INACTIVE.toStdString())
            {
                return QBrush(Qt::red);
            }
            else if (devices_.at(index.row()).status == Status::DISABLED.toStdString())
            {
                return QBrush(Qt::gray);
            }

        }
        else if (index.column() == static_cast<int>(DeviceModelColumns::NAME_COLUMN))
        {
            return QBrush(Qt::lightGray);
        }
    }
    else if (role == Qt::FontRole)
    {
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        return font;
    }
    return QVariant();
}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal && this->rowCount() > 0)
        {
            switch(section)
            {
            case static_cast<int>(DeviceModelColumns::NAME_COLUMN):
                return QObject::tr("Name");
            case static_cast<int>(DeviceModelColumns::STATUS_COLUMN):
                return QObject::tr("Status");
            case static_cast<int>(DeviceModelColumns::MESSAGE_COLUMN):
                return QObject::tr("Info");
            default:
                return QObject::tr("Unknown");
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags DeviceModel::flags(const QModelIndex &index) const
{
    if (index.column() == static_cast<int>(DeviceModelColumns::NAME_COLUMN))
    {
        return Qt::ItemFlags {Qt::ItemIsSelectable, Qt::ItemIsEnabled};
    }
    else if (index.column() == static_cast<int>(DeviceModelColumns::STATUS_COLUMN))
    {
        return Qt::ItemFlags {Qt::ItemIsEnabled, Qt::ItemIsEditable};
    }
    else if (index.column() == static_cast<int>(DeviceModelColumns::MESSAGE_COLUMN))
    {
        return Qt::ItemFlags {Qt::ItemIsEnabled};
    }
    return {};
}

void DeviceModel::setDevices(QList<DeviceItem> devices)
{
    if (!devices.isEmpty())
    {
        this->removeDevices();
        beginInsertRows(QModelIndex(), 0, devices.size() - 1);
        devices_ = devices;
        emit dataChanged(QModelIndex(), index(devices_.size(), this->columns_ - 1));
        endInsertRows();
    }
}

void DeviceModel::removeDevices()
{
    if (!devices_.empty())
    {
        int size = devices_.size();
        beginRemoveRows(QModelIndex(), 0, size - 1);
        devices_ = {};
        emit dataChanged(QModelIndex(), index(this->rowCount(), this->columns_));
        endRemoveRows();
    }
    changedIndexes_ = {};
}

void DeviceModel::changeDeviceStatus(const int index, const QString status)
{
    devices_[index].status = status.toStdString();
    changedIndexes_.append(this->index(index, static_cast<int>(DeviceModelColumns::STATUS_COLUMN)));
    emit dataChanged(this->index(index, 0), this->index(index, this->columns_));
}

bool DeviceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == static_cast<int>(DeviceModelColumns::STATUS_COLUMN)
            && value.toString() != QString::fromStdString(devices_[index.row()].status))
    {
        this->changeDeviceStatus(index.row(), value.toString());
        return true;
    }
    return false;
}

QJsonObject DeviceModel::getJsonDeviceChange(const QModelIndex &index) const
{
    QJsonObject obj;
    obj.insert("username", QString::fromStdString(devices_.at(index.row()).name));
    obj.insert("status", QString::fromStdString(devices_.at(index.row()).status));
    return obj;
}

bool DeviceModel::isDeviceChanged(const int index) const
{
    QModelIndex mi = this->index(index, static_cast<int>(DeviceModelColumns::STATUS_COLUMN));
    return changedIndexes_.contains(mi);
}

bool DeviceModel::removeDeviceIfChanged(const int index)
{
    QModelIndex mi = this->index(index, static_cast<int>(DeviceModelColumns::STATUS_COLUMN));
    if (changedIndexes_.contains(mi))
    {
        changedIndexes_.removeAll(mi);
        emit this->dataChanged(this->index(index, static_cast<int>(DeviceModelColumns::STATUS_COLUMN)),
                               this->index(index, static_cast<int>(DeviceModelColumns::STATUS_COLUMN)));
        return true;
    }
    else
    {
        return false;
    }
}

void DeviceModel::removeChangedDevices()
{
    changedIndexes_ = {};
    emit this->dataChanged(this->index(0, static_cast<int>(DeviceModelColumns::STATUS_COLUMN)),
                           this->index(this->rowCount(), static_cast<int>(DeviceModelColumns::STATUS_COLUMN)));
}

QString DeviceModel::getDeviceName(const int index) const
{
    return QString::fromStdString(devices_.at(index).name);
}

QList<QString> DeviceModel::getDevices() const
{
    QList<QString> devices;
    for (DeviceItem device : devices_)
    {
        devices.append(QString::fromStdString(device.name));
    }
    return devices;
}

QList<QModelIndex> DeviceModel::getChangedIndexes() const
{
    return changedIndexes_;
}

QString DeviceModel::getDeviceMessage(const int index) const
{
    return QString::fromStdString(devices_.at(index).message);
}

QString DeviceModel::getDeviceProgramm(const int index) const
{
    return QString::fromStdString(devices_.at(index).programm);
}

const QString Status::ACTIVE = "ACTIVE";
const QString Status::DISABLED = "DISABLED";
const QString Status::INACTIVE = "INACTIVE";

QWidget* StatusDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *result = new QComboBox(parent);
    if (statusModel != nullptr)
    {
        result->setModel(statusModel);
    }
    result->setGeometry(option.rect);
    return result;
}

void StatusDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *result = qobject_cast<QComboBox*>(editor);
    result->setCurrentIndex(result->findData(index.data(), Qt::DisplayRole));
}

void StatusDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *result = qobject_cast<QComboBox*>(editor);
    if (result->currentIndex() >= 0)
    {
        model->setData(index, result->currentText());
    }
}

void StatusDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

StatusModel::StatusModel(QObject *parent) : QAbstractListModel(parent)
{
    data_.append({Status::ACTIVE, "active"});
    data_.append({Status::DISABLED, "disabled"});
    data_.append({Status::INACTIVE, "inactive"});
}

int StatusModel::rowCount(const QModelIndex &) const
{
    return data_.size();
}

QVariant StatusModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return data_[index.row()].first;
    }
    else if (role == Qt::UserRole)
    {
        return data_[index.row()].second;
    }
    return QVariant();
}

QString StatusModel::getStatus(const int index) const
{
    return this->data_.at(index).first;
}
