#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QList>
#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include "device.h"
#include "deviceicmp.h"
#include "patcherfunc.h"
#include "commandwind.h"
#include "devicemodel.h"
#include "devicedelegate.h"
#include "sshclient.h"
#include "sshprocess.h"
#include "sshsftp.h"
#include "sshsftpcommandsend.h"
#include "devicedelegate.h"
#include "sshscpsend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    DeviceModel* m_model = nullptr;
    QTranslator m_tranclator;
    SshClient m_client;
    SshProcess* m_proc = nullptr;
    SshSFtp* m_sftp = nullptr;
    QThread* m_thread = nullptr;
    DeviceIcmp* m_deviceIcmp = nullptr;
    QTimer* m_timerStatus = nullptr;
    QSettings* m_settings = nullptr;
    DeviceIcmp* getIcmpHandler();
    SshScpSend* m_scp = nullptr;

    void startPatching();
    void createConnections();
public:
    void createRow(Device* dev);
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

protected:
    void changeEvent(QEvent* event) override;

private slots:
    void on_actionRussian_triggered();
    void on_actionEnglish_triggered();
    void on_But_add_device_clicked();
    void on_But_start_pach_clicked();
    void save_setting_device();
    void create_device_settings();
    void on_actionLoad_settings_triggered();
    void on_actionSave_sattings_triggered();
    void on_but_del_device_clicked();
    void setStatus(bool, Device*);
    void loadSettings();
    void saveSettings();
    void receive_command(const QStringList&);
    void polling();

signals:
    void get_command(const QStringList&);
    void ask_status(const QString&,Device*);
private:
    Ui::MainWindow *ui;
};


