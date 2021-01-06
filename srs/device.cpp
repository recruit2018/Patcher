#include "device.h"

Device::Device(QObject *parent) : QObject(parent)
{

}

QString Device::getvalue(const int &val) const
{
    switch (val)
    {
    case Columns::Address:{return get_host(); break;}
    case Columns::User:{return get_user();  break;}
    case Columns::DeviceName:{return get_device_name(); break;}
    case Columns::Port:{return QString::number(get_port()); break;}
    }
}

void Device::setvalue(const QString& val, const int &column)
{
    switch (column)
    {
    case Columns::Address:{set_addr(val); break;}
    case Columns::User:{set_user(val);  break;}
    case Columns::DeviceName:{set_device_name(val); break;}
    case Columns::Port:{set_port(val.toUInt()); break;}
    };
}

const void Device::printself()
{
    qDebug()<<m_user<<m_device_name<<m_host<<m_port<<"\n";
    Patcher::printlist(m_shellcommand);
    qDebug()<<"PATH: "<<m_localfilepath<<"  "<<m_destfilepath<<"\n"<<"\n";

}
