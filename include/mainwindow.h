#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QFileDialog>
#include <QSettings>
#include <QList>
#include <QMessageBox>
#include "device.h"
#include "sshclient.h"
#include "sshprocess.h"
#include "sshsftp.h"
#include "sshsftpcommandsend.h"
#include "patcherfunc.h"
#include "jsoncontroller.h"

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
    QString m_user;
    QString m_device;
    QString m_host;
    QString m_pass;
    QByteArrayList MyByteList;
    QSettings* m_settings;
    QTranslator m_tranclator;
    QString m_localfilepath;
    QString m_destfilepath;
    QList<Device*> m_device_list;

    bool save_device_settings();
    bool loadSettings();
    void saveSettings();
    void createRow(Device* dev = nullptr);
    inline void addDevice() { m_device_list.append(new Device(this)); }
protected:
    void changeEvent(QEvent* event) override;
public:
    MainWindow(QWidget *parent = nullptr);
   virtual ~MainWindow();

private slots:
    void on_actionRussian_triggered();

    void on_actionEnglish_triggered();

    void on_But_add_device_clicked();

    void on_But_start_pach_clicked();

    void save_setting_device();

    void on_actionLoad_settings_triggered();

    void on_actionSave_sattings_triggered();

    void on_but_del_device_clicked();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
