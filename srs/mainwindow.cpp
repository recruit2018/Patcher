#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{  
    ui->setupUi(this);
    m_model = new DeviceModel(this);
    ui->t
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_But_start_pach_clicked()
{

    ui->But_add_device->setEnabled(false);
    ui->But_start_pach->setEnabled(false);
    ui->but_del_device->setEnabled(false);

    Device * dev;
    QStringList command;
    SshSFtp* sftp;
    SshProcess* proc;
    SshSftpCommandSend* cmd;
    QByteArrayList MyByteList;
    MyByteList.append("password");

    QString source("./newpatch.tar.gz");
    QString dest("/home/ivan/Remotemachine/newpatch.tar.gz");

    QString localfilehash;
    QString filehash;
    QString filecount;

    for(int i= 0; i < m_device_list.count(); i++)
    {
        dev = m_device_list.at(i);

        m_client.setPassphrase(ui->commandtableWidget->item(i,3)->text());

        QEventLoop waitssh;
        QObject::connect(&m_client, &SshClient::sshReady, &waitssh, &QEventLoop::quit);
        QObject::connect(&m_client, &SshClient::sshError, &waitssh, &QEventLoop::quit);
        m_client.connectToHost(dev->get_user(),dev->get_host(),dev->get_port(),MyByteList);
        waitssh.exec();

        if(m_client.sshState() != SshClient::SshState::Ready)
        {
            qDebug() << "Can't connect to connexion server";
        }else
            qDebug()  << "SSH connected";


        command = dev->get_command_list();
        for(int j= 0; j < command.count();j++)
        {
            //            proc = m_client.getChannel<SshProcess>(QString("command%1").arg(i));
            //            if(j == 0)
            //            {
            //                proc->runCommand(command.at(j));
            //                localfilehash = Patcher::getfilehash("./1.txt");
            //                filehash = QString::fromStdString(proc->result().toStdString());
            //                filehash.chop(1);


            //                if(filehash != localfilehash)
            //                {
            //                    qDebug()<<filehash<<"   "<<localfilehash;
            //                    sftp = m_client.getChannel<SshSFtp>("ftp");
            //                    cmd = new SshSftpCommandSend(source,dest,*sftp);
            //                    QEventLoop wait;
            //                    QObject::connect(sftp, &SshSFtp::finish, &wait, &QEventLoop::quit);
            //                    sftp->processCmd(cmd);
            //                    wait.exec();
            //                }
            //                continue;
            //            }

            //            if( j == 2 )
            //            {
            //                proc->runCommand(command.at(j));
            //                filecount = QString::fromStdString(proc->result().toStdString());
            //                ++j;
            //                if(filecount.toInt() > 10)
            //                {
            //                    proc = m_client.getChannel<SshProcess>(QString("command%1").arg(j));
            //                    proc->runCommand(command.at(j));

            //                }
            //                continue;
            //            }
            if(command.at(j).indexOf(QRegExp("^sftp .+")))
            {
                cmd = new SshSftpCommandSend(dev->get_sftp_local_path(),dev->get_sftp_remote_path(),*sftp);
                sftp = m_client.getChannel<SshSFtp>("ftp");
                cmd = new SshSftpCommandSend(source,dest,*sftp);
                QEventLoop wait;
                QObject::connect(sftp, &SshSFtp::finish, &wait, &QEventLoop::quit);
                sftp->processCmd(cmd);
                wait.exec();
            }
            proc->runCommand(command.at(j));
        }

        proc = m_client.getChannel<SshProcess>("fsfdfsdf");
        proc->runCommand("ls");
        m_client.disconnectFromHost();
    }

    ui->But_add_device->setEnabled(true);
    ui->But_start_pach->setEnabled(true);
    ui->but_del_device->setEnabled(true);

}



void MainWindow::save_setting_device()
{

    ui->commandtableWidget->save_device_settings();
}

void MainWindow::create_setting_device()
{
    CommandWind* wind = new CommandWind(this);
    wind->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(wind,SIGNAL(send_command(const QString&)),ui->commandtableWidget,SLOT(recive_command(const QString&)));
    connect(this,SIGNAL(get_command(QString&)),wind,SLOT(data_recived(QString&)));
    wind->show();

    ui->commandtableWidget->create_device_settings();
}

void MainWindow::on_But_add_device_clicked()
{

    ui->commandtableWidget->addDevice();
    if(ui->commandtableWidget->rowCount() > 0)
    {
        ui->but_del_device->setEnabled(true);
        ui->But_start_pach->setEnabled(true);
    }
}

void MainWindow::on_actionRussian_triggered()
{
    m_tranclator.load("patcher_ru_RU",".");
    qApp->installTranslator(&m_tranclator);
}



void MainWindow::changeEvent(QEvent* event)
{

    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    else
        QMainWindow::changeEvent(event);
}

void MainWindow::on_actionEnglish_triggered()
{
    qApp->removeTranslator(&m_tranclator);
}


void MainWindow::on_actionLoad_settings_triggered()
{
    int ret = QMessageBox::warning(this, tr("Patcher"),
                                   tr("When downloading, all unsaved data will be lost.\n"
                                      "Do you really want to continue?"),
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if(ret == QMessageBox::Ok)
    {
        ui->commandtableWidget->setRowCount(0);
        ui->commandtableWidget->loadSettings();
    }
    else
        return;

    if(ui->commandtableWidget->rowCount() > 0)
    {
        ui->but_del_device->setEnabled(true);
        ui->But_start_pach->setEnabled(true);
    }
}

void MainWindow::on_actionSave_sattings_triggered()
{
    int ret = QMessageBox::warning(this, tr("Patcher"),
                                   tr("When saving, the previous settings will be permanently changed.\n"
                                      "Do you really want to continue?"),
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if(ret == QMessageBox::Ok)
    {
        ui->commandtableWidget->saveSettings();
    }
    else
        return;
}

void MainWindow::on_but_del_device_clicked()
{     
    ui->commandtableWidget->delCurRow();

    if(ui->commandtableWidget->rowCount() == 0)
    {
        ui->but_del_device->setEnabled(false);
        ui->But_start_pach->setEnabled(false);
    }
}


