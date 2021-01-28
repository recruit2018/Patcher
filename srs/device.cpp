#include "device.h"

Device::Device(QObject *parent) : QObject(parent)
{

}



void Device::printself() const
{
    qDebug()<<m_user<<m_device_name<<m_host<<m_port<<"\n";
    qDebug()<<"PATH: "<<m_localfilepath<<"  "<<m_destfilepath<<"\n"<<"\n";

}
