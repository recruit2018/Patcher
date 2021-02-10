#include "device.h"

Device::Device(QObject *parent) : QObject(parent)
{
}


void Device::printself() const
{
    qDebug()<<m_user<<m_device_name<<m_addr<<m_port<<"\n";
    Patcher::printlist(m_shellcommand);
    qDebug()<<"PATH: "<<m_localfilepath<<"  "<<m_remotefilepath<<"\n"<<"\n";
}
