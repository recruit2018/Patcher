#include "devicemodel.h"



DeviceModel::DeviceModel(QObject * parent)
    :QAbstractTableModel(parent)
{

}

int DeviceModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_device_list.count();
}

int DeviceModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 9;
}

QVariant DeviceModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() ||
    index.row() < 0 || index.row() >= m_device_list.count() ||
    index.column() < 0 || index.column() >= 9)
    return QVariant();

    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignLeft | Qt::AlignVCenter);

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Device* dev;
        dev = m_device_list.at(index.row());
        switch (index.column())
        {
        case Address: {return dev->get_host(); break;}
        case User: {return dev->get_user();  break;}
        case DeviceName: {return dev->get_device_name(); break;}
        case Port: {return QString::number(dev->get_port()); break;}
        case Status: {return dev->get_status(); break;}
        case Stage: {return dev->get_stage(); break;}
        //default: Q_ASSERT(false); !!!!!!!
        }        
    }
    else
        return QVariant();
}


bool DeviceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
    index.row() < 0 || index.row() >= m_device_list.count() ||
    index.column() < 0 || index.column() >= 9)
    return false;

        Device* dev;
        dev = m_device_list.at(index.row());
        switch (index.column())
        {
        case Address: {dev->set_addr(value.toString()); break;}
        case User: {dev->set_user(value.toString());  break;}
        case DeviceName: {dev->set_device_name(value.toString()); break;}
        case Port: {dev->set_port(value.toUInt()); break;}
        case Status: {dev->set_status(value.toString()); break;}
        case Stage: {dev->set_stage(value.toString()); break;}
        default: Q_ASSERT(false);
        };

        emit dataChanged(index, index);

        return true;
}

Qt::ItemFlags DeviceModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return flags;
}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{   
    if(role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case Columns::Address: return tr("Ip Address");
        case Columns::User: return tr("User");
        case Columns::DeviceName: return tr("Device name");
        case Columns::Password: return tr("Password");
        case Columns::Port: return tr("Port");
        case Columns::Status: return tr("Status");
        case Columns::Stage: return tr("Stage");
        case Columns::butCRTSettings: return tr("Create setting");
        case Columns::butSVSettings: return tr("Save setting");
        default: Q_ASSERT(false);
        }
    }
    return section + 1;
}


bool DeviceModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row ,row+count-1);

    for (int i = 0; i < count; ++i)
    {
        m_device_list.insert(row, new Device(this));
    }
    endInsertRows();
    return true;
}

bool DeviceModel::removeRows(int row, int count, const QModelIndex&)
{    
    beginRemoveRows(QModelIndex(), row, row+count-1);

    for (int i = 0; i < count; ++i)
    {
        m_device_list.removeAt(row);
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

Device* DeviceModel::getDevice(int pos)
{
    return m_device_list.at(pos);
}



int DeviceModel::deviceExist(Device* dev)
{
    return m_device_list.indexOf(dev);
}



