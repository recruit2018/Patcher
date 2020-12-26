#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QFileDialog>
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
    QString m_host;
    QString m_pass;
    QByteArrayList MyByteList;
    QTranslator m_tranclator;
    QString m_localfilepath;
    QString m_destfilepath;
    int getconnectioninfo();

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

    void on_but_load_command_clicked();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
