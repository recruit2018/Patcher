# pragma once

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDir>
#include <QPushButton>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include "device.h"
#include "sshclient.h"
#include "commandwind.h"
#include "patcherfunc.h"
#include "sshprocess.h"
#include "sshsftp.h"
#include "sshsftpcommandsend.h"



class CommandTable : public QTableWidget
{
    Q_OBJECT
private:    
    QList<Device*> m_device_list;
    QSettings* m_settings;
    SshClient m_client;
    SshProcess* m_proc;
    SshSFtp* m_sftp;
    QThread* m_thread;

public:
    enum Columns {
        Address,
        User,
        DeviceName,
        Password,
        Port,
        Stage,
        Status,
        butCRTSettings,
        butSVSettings
    };
    Q_ENUM(Columns)

    CommandTable(QWidget *parent);

    void createRow(Device*);
    void addDevice();
    void saveSettings();
    void loadSettings();
    void delCurRow();
    void getDeviceList();
    void startPatching();

private slots:
    void recive_command(const QString&);

    void somethingChanged(QTableWidgetItem*);

public slots:
    void create_device_settings();

    void save_device_settings();

signals:
    void get_command(QString&);
};


