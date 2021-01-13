#include "deviceicmp.h"

DeviceIcmp::DeviceIcmp(QObject* parent): QObject(parent)
{

}

void DeviceIcmp::getStatus(const QString& str, int index)
{    
    ipaddr = inet_addr(str.toStdString().c_str());
    hIcmpFile = IcmpCreateFile();

    // Выделяем память под буффер ответов
    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);

    // Вызываем функцию ICMP эхо запроса
    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
                            NULL, ReplyBuffer, ReplySize, 1000);

    if (dwRetVal != 0)
    {
        emit send_status(true,index);
    }
    else
    {
        emit send_status(false,index);
    }

    IcmpCloseHandle(hIcmpFile);
    free(ReplyBuffer); // Освобождаем память
}
