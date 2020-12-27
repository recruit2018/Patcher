#ifndef DEVICE_H
#define DEVICE_H
#include <QObject>
#include <QStringList>
#include "patcherfunc.h"
#include <QtDebug>

class Device : public QObject
{
    Q_OBJECT
private:
    QString m_user{"ivan"};
    QString m_device_name{"localhost"};
    QString m_host{"127.0.0.1"};
   // QString m_pass;
    quint16 m_port{22};
    QStringList m_shellcommand;

public:
    explicit Device(QObject *parent = nullptr);
    void set_user(const QString& user) { m_user = user; }
    void set_device_name(const QString& devname) { m_device_name = devname; }
    void set_addr(const QString& addr) { m_host = addr; }
    void set_port (const quint16& port) { m_port = port; }
    void set_command_list(const QStringList& comm) { m_shellcommand = comm; }
    void printobject();

signals:

};

#endif // DEVICE_H
