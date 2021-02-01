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
    QString m_lineStatus{"unknown"};
    QString m_stage{"wait for start"};

public:

    enum Columns {
        Address,
        User,
        DeviceName,
        Port
    };
    Q_ENUM(Columns)
    explicit Device(QObject *parent = nullptr);

    inline void set_user(const QString& user) { m_user = user; }
    inline void set_device_name(const QString& devname) { m_device_name = devname; }
    inline void set_addr(const QString& addr) { m_host = addr; }
    inline void set_port (const quint16& port) { m_port = port; }
    inline void set_status(const QString& status) { m_lineStatus = status; }
    inline void set_stage(const QString& stage) { m_stage = stage; }
    inline void add_command(const QString& command) { m_shellcommand.append(command); }
    inline void set_command_list(const QStringList& comm) { m_shellcommand = comm; }
    inline void set_sftp_local_path(const QString& local) { m_localfilepath = local; }
    inline void set_sftp_remote_path(const QString& remote) { m_destfilepath = remote; }
    inline QString get_user() const { return m_user; }
    inline QString get_status() const { return m_lineStatus; }
    inline QString get_stage() const { return m_stage; }
    inline QString get_device_name() const { return m_device_name; }
    inline QString get_host() const { return m_host; }
    inline quint16 get_port() const { return m_port; }
    inline QStringList get_command_list() const { return m_shellcommand; }
    inline QString get_sftp_local_path() const { return m_localfilepath; }
    inline QString get_sftp_remote_path() const { return m_destfilepath; }

    QString getvalue(const int&) const;
    void setvalue(const QString &, const int& column);
    void printself() const;

signals:

};

