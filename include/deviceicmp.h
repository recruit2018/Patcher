#pragma once

#include <QObject>
#include <QString>
#include <QDebug>

#ifdef _WIN64
#include "iphlpapi.h"
#include "icmpapi.h"
#include <winsock2.h>
#endif

class DeviceIcmp : public QObject
{
    Q_OBJECT
public:
    DeviceIcmp(QObject* = nullptr);
    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32];
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;
public slots:
    void getStatus(const QString&, int);

signals:
    void send_status(bool,int);
};


