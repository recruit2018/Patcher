#include "devicemodel.h"

DeviceModel::DeviceModel(QObject * parent)
    :QAbstractTableModel(parent)
{
    setHeaders();
}

int DeviceModel::rowCount(const QModelIndex&) const
{
    return m_device_list.count();
}

int DeviceModel::columnCount(const QModelIndex&) const
{
    return m_device_list.count();
}

QVariant DeviceModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignRight | Qt::AlignCenter);
    else if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return getvalue(index.row(),index.column());
    }
    else
        return QVariant();
}

QVariant DeviceModel::getvalue(const int& row, const int& column) const
{
    Device* dev;
    dev = m_device_list.at(row);
    return dev->getvalue(column);
}

void DeviceModel::setvalue(const QString& val, const int &column)
{
    Device* dev;
    dev = m_device_list.at(column);
    dev->setvalue(val,column);
}

bool DeviceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(role == Qt::EditRole)
    {
        setvalue(value.toString(),index.column());
        QModelIndex item = createIndex(index.row(),index.column());
        emit dataChanged(index, index);
        emit dataChanged(item, item);
        return true;
    }
    return false;
}

Qt::ItemFlags DeviceModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags = Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return flags;
}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
        return m_headers.at(section);
    else
        return QVariant();
}

void DeviceModel::setHeaders()
{
    m_headers << tr("Ip Address")<< tr("User")<< tr("Device name")
              << tr("Password")<< tr("Port")<< tr("Stage")
              << tr("Status")<< tr("Create setting")<< tr("Save setting");
}


