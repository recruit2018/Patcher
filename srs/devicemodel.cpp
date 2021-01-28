#include "devicemodel.h"

DeviceModel::DeviceModel(QObject * parent)
    :QAbstractTableModel(parent)
{
    setHeaders();
}

int DeviceModel::rowCount(const QModelIndex&parent) const
{
    return m_device_list.count();
}

int DeviceModel::columnCount(const QModelIndex&parent) const
{
    return m_headers.count();
}

QVariant DeviceModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignLeft | Qt::AlignCenter);
    else if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Device* dev;
        dev = m_device_list.at(index.row());
        switch (index.column())
        {
        case Columns::Address:{return dev->get_host(); break;}
        case Columns::User:{return dev->get_user();  break;}
        case Columns::DeviceName:{return dev->get_device_name(); break;}
        case Columns::Port:{return QString::number(dev->get_port()); break;}
        }
        return QString("");
    }
    else
        return QVariant();
}


bool DeviceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(role == Qt::EditRole)
    {

        Device* dev;
        dev = m_device_list.at(index.row());
        switch (index.column())
        {
        case Columns::Address:{dev->set_addr(value.toString()); break;}
        case Columns::User:{dev->set_user(value.toString());  break;}
        case Columns::DeviceName:{dev->set_device_name(value.toString()); break;}
        case Columns::Port:{dev->set_port(value.toUInt()); break;}
        };
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
        return (section+1);
}

void DeviceModel::setHeaders()
{
    m_headers << tr("Ip Address")<< tr("User")<< tr("Device name")
              << tr("Password")<< tr("Port")<< tr("Stage")
              << tr("Status")<< tr("Create setting")<< tr("Save setting");
}

bool DeviceModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(),position,position+rows-1);
    Device* dev =new Device(this);

    for (int row = 0; row < rows; ++row)
    {
        m_device_list.append(dev);
    }

    endInsertRows();

    return true;
}

bool DeviceModel::removeRows(int position, int rows, const QModelIndex &index)
{
    if(!(index.isValid()))
        return false;

    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row)
    {
        m_device_list.removeAt(position);
    }
    endRemoveRows();
    return true;
}


void DeviceModel::createRow(Device* dev)
{
    beginInsertRows(QModelIndex(),m_device_list.count(),m_device_list.count());
    m_device_list.append(dev);

    endInsertRows();
    emit(dataChanged(index(m_device_list.count(), 0), index(m_device_list.count(), 2)));

}



