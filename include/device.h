# pragma once

#include <QObject>
#include <QtDebug>
#include "patcherfunc.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);    
    Device (const Device&) = delete;
    Device& operator=(const Device&) = delete;

    QString m_user{"ivan"};
    QString m_device_name{"localhost"};
    QString m_addr{"127.0.0.1"};
    quint16 m_port{22};
    QStringList m_shellcommand;
    QString m_localfilepath;
    QString m_remotefilepath;
    QString m_onlineStatus{"unknown"};
    QString m_stage{"wait for start"};
    QString m_pass;
    inline void add_command(const QString& command) { m_shellcommand.append(command); }
    void printself() const; //temp func for debug
};

