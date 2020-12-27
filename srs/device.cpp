#include "device.h"

Device::Device(QObject *parent) : QObject(parent)
{
    m_shellcommand = Patcher::jspursing("commandshell.json");
}

void Device::printobject()
{
    qDebug()<<m_user<<m_device_name<<m_host<<m_port<<"\n"<<
    "SHELL COMMANDS:"<<"\n"<<m_shellcommand;
}
