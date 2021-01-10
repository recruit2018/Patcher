#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include "device.h"
#include "sshclient.h"
#include "sshprocess.h"
#include "sshsftp.h"
#include "sshsftpcommandsend.h"
#include "patcherfunc.h"
#include "commandwind.h"
#include "devicemodel.h"
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    SshClient m_client;
    SshProcess *m_proc;
    SshSFtp *m_sftp;
    QStringList m_shellcommand;
    QTranslator m_tranclator;
    QString m_localfilepath;
    QString m_destfilepath;
    QList<Device*> m_device_list;
    DeviceModel* m_model;
    QSettings* m_settings;
protected:
    void changeEvent(QEvent* event) override;
public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    void saveSettings();
    void loadSettings();



private slots:
    void on_actionRussian_triggered();

    void on_actionEnglish_triggered();

    void on_But_add_device_clicked();

    void on_But_start_pach_clicked();

    void save_setting_device();

    void create_setting_device();

    void on_actionLoad_settings_triggered();

    void on_actionSave_sattings_triggered();

    void on_but_del_device_clicked();
    void create_device_settings();


    void recive_command(const QString& command);

signals:
    void get_command(QString&);

private:
    Ui::MainWindow *ui;
};


