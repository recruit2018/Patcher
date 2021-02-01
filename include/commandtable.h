//# pragma once

//#include <QTableWidget>
//#include <QTableWidgetItem>
//#include <QSettings>
//#include <QString>
//#include <QStringList>
//#include <QList>
//#include <QDir>
//#include <QPushButton>
//#include <QDebug>
//#include <QTimer>
//#include <QThread>
//#include <QMutex>
//#include "device.h"
//#include "sshclient.h"
//#include "commandwind.h"
//#include "patcherfunc.h"
//#include "sshprocess.h"
//#include "deviceicmp.h"
//#include "sshsftp.h"
//#include "sshsftpcommandsend.h"



//class CommandTable : public QTableWidget
//{
//    Q_OBJECT
//private:
//    QList<Device*> m_device_list;
//    QSettings* m_settings;
//    SshClient m_client;
//    SshProcess* m_proc;
//    SshSFtp* m_sftp;
//    QThread* m_thread;
//    DeviceIcmp* m_deviceIcmp;
//    QTimer* m_timerStatus;
//    QMutex m_mutex;

//    DeviceIcmp* getIcmpHandler();

//public:
//    enum Columns {
//        Address,
//        User,
//        DeviceName,
//        Password,
//        Port,
//        Status,
//        Stage,
//        butCRTSettings,
//        butSVSettings
//    };
//    Q_ENUM(Columns)

//    CommandTable(QWidget *parent);

//    void createRow(Device*);
//    void addDevice();
//    void saveSettings();
//    void loadSettings();
//    void delCurRow();
//    void getDeviceList();
//    void startPatching();
//    virtual ~CommandTable();

//private slots:
//    void recive_command(const QStringList&);
//    void polling();
//    void somethingChanged(QTableWidgetItem*);
//    void setStatus(bool,Device*);

//public slots:
//    void create_device_settings();

//    void save_device_settings();

//signals:
//    void get_command(const QStringList&);
//    void ask_status(const QString&,Device*);
//};


