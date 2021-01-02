# pragma once

#include <QObject>
#include <QStringList>
#include <QtDebug>
#include "patcherfunc.h"

class Device : public QObject
{
    Q_OBJECT
private:
    QString m_user{"ivan"};
    QString m_device_name{"localhost"};
    QString m_host{"127.0.0.1"};
    quint16 m_port{22};

    QStringList m_shellcommand;

    QString m_localfilepath;
    QString m_destfilepath;

public:
    explicit Device(QObject *parent = nullptr);
    inline void set_user(const QString& user) { m_user = user; }
    inline void set_device_name(const QString& devname) { m_device_name = devname; }
    inline void set_addr(const QString& addr) { m_host = addr; }
    inline void set_port (const quint16& port) { m_port = port; }
    inline void add_command(const QString& command) { m_shellcommand.append(command); }
    inline void set_command_list(const QStringList& comm) { m_shellcommand = comm; }
    inline void set_sftp_local_path(const QString& local) { m_localfilepath = local; }
    inline void set_sftp_remote_path(const QString& remote) { m_destfilepath = remote; }
    inline  const QString get_user() { return m_user; }
    inline const QString get_device_name() { return m_device_name; }
    inline const QString get_host() { return m_host; }
    inline const quint16 get_port() { return m_port; }
    inline const QStringList get_command_list() { return m_shellcommand; }
    inline const QString get_sftp_local_path() { return m_localfilepath; }
    inline const QString get_sftp_remote_path() { return m_destfilepath; }

    const void printself();

signals:

};

