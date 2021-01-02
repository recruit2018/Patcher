#include "device.h"

Device::Device(QObject *parent) : QObject(parent)
{
     m_shellcommand = Patcher::jspursing("commandshell.json");
}

const void Device::printself()
{
    qDebug()<<m_user<<m_device_name<<m_host<<m_port<<"\n";
    Patcher::printlist(m_shellcommand);
    qDebug()<<"PATH: "<<m_localfilepath<<"  "<<m_destfilepath<<"\n"<<"\n";

}
