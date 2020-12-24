#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
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
protected:
    void changeEvent(QEvent* event) override;
public:
    MainWindow(QWidget *parent = nullptr);
   virtual ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionRussian_triggered();

    void on_actionEnglish_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
