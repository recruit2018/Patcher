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

    QString m_localfilepath;
    QString m_destfilepath;

public:
    explicit Device(QObject *parent = nullptr);
    void set_user(const QString& user) { m_user = user; }
    void set_device_name(const QString& devname) { m_device_name = devname; }
    void set_addr(const QString& addr) { m_host = addr; }
    void set_port (const quint16& port) { m_port = port; }
    void add_command(const QString& command) { m_shellcommand.append(command); }
    void set_command_list(const QStringList& comm) { m_shellcommand = comm; }
    void set_sftp_local_path(const QString& local) { m_localfilepath = local; }
    void set_sftp_remote_path(const QString& remote) { m_destfilepath = remote; }
    QString get_user() { return m_user; }
    QString get_device_name() { return m_device_name; }
    QString get_host() { return m_host; }
    quint16 get_port() { return m_port; }
    void printself();

signals:

};

#endif // DEVICE_H
