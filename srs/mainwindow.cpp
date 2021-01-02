#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_settings = new QSettings(QDir::currentPath()+"/settings.ini", QSettings::IniFormat, this);

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
            proc = m_client.getChannel<SshProcess>(QString("command%1").arg(i));
            if(j == 0)
            {
                proc->runCommand(command.at(j));
                localfilehash = Patcher::getfilehash("./1.txt");
                filehash = QString::fromStdString(proc->result().toStdString());
                filehash.chop(1);

                // source = dev->get_sftp_local_path();
                //  dest = dev->get_sftp_remote_path();

                if(filehash != localfilehash)
                {
                    qDebug()<<filehash<<"   "<<localfilehash;
                    sftp = m_client.getChannel<SshSFtp>("ftp");
                    cmd = new SshSftpCommandSend(source,dest,*sftp);
                    QEventLoop wait;
                    QObject::connect(sftp, &SshSFtp::finish, &wait, &QEventLoop::quit);
                    sftp->processCmd(cmd);
                    wait.exec();
                }
                continue;
            }

            if( j == 2 )
            {
                proc->runCommand(command.at(j));
                filecount = QString::fromStdString(proc->result().toStdString());
                ++j;
                if(filecount.toInt() > 10)
                {
                    proc = m_client.getChannel<SshProcess>(QString("command%1").arg(j));
                    proc->runCommand(command.at(j));

                }
                continue;
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

//    m_shellcommand = Patcher::jspursing("commandshell.json");
//    m_client.setPassphrase(m_pass);
//    QEventLoop waitssh;
//    QObject::connect(&m_client, &SshClient::sshReady, &waitssh, &QEventLoop::quit);
//    QObject::connect(&m_client, &SshClient::sshError, &waitssh, &QEventLoop::quit);
//    m_client.connectToHost(m_user,m_host,22,MyByteList);
//    waitssh.exec();

//    if(m_client.sshState() != SshClient::SshState::Ready)
//     {
//          qDebug() << "Can't connect to connexion server";
//     }else
//          qDebug()  << "SSH connected";


//               SshProcess *proc = m_client.getChannel<SshProcess>("command");

//               proc->runCommand(m_shellcommand.at(0));

//               QString localfilehash = Patcher::getfilehash("./1.txt");

//               QString filehash = QString::fromStdString(proc->result().toStdString());

//               filehash.chop(1);

//                  const QString source("./newpatch.tar.gz");
//                  QString dest("/home/ivan/Remotemachine/newpatch.tar.gz");

//         if(filehash != localfilehash)
//          {
//               qDebug()<<filehash<<"   "<<localfilehash;
//               QEventLoop wait;

//               SshSFtp *mysftp = m_client.getChannel<SshSFtp>("mysftp!");
//               SshSftpCommandSend *cmd = new SshSftpCommandSend(source,dest,*mysftp);
//               QObject::connect(mysftp, &SshSFtp::finish, &wait, &QEventLoop::quit);

//               mysftp->processCmd(cmd);
//               wait.exec();
//           }

//             SshProcess *procc = m_client.getChannel<SshProcess>("command2");


//             procc->runCommand(m_shellcommand.at(1));


//             SshProcess *proccc = m_client.getChannel<SshProcess>("command3");

//             proccc->runCommand(m_shellcommand.at(2));
//             QString filecount = QString::fromStdString(proccc->result().toStdString());

//             if(filecount.toInt() > 10)
//             {

//             SshProcess *procccc = m_client.getChannel<SshProcess>("command4");
//             procccc->runCommand(m_shellcommand.at(3));

//             }

//             SshProcess *procccc = m_client.getChannel<SshProcess>("command5");
//             procccc->runCommand("ls");


////             m_client.disconnectFromHost();
////                 sleep(10);
////                  client = new SshClient("FIRST CLIENT2");
////                 client->setPassphrase(cmdargs[3]);

////                 QEventLoop waitsshh;
////                 QObject::connect(client, &SshClient::sshReady, &waitsshh, &QEventLoop::quit);
////                 QObject::connect(client, &SshClient::sshError, &waitsshh, &QEventLoop::quit);
////                 client->connectToHost(cmdargs[1],cmdargs[2],myport,MyByteList);
////                 waitsshh.exec();

////                 if(client->sshState() != SshClient::SshState::Ready)
////                  {
////                       qDebug() << "Can't connect to connexion server";

////                  }else
////                       qDebug()  << "SSH connected";

//                 //SshProcess *proccccc = client->getChannel<SshProcess>("command5");
//                 //proccccc->runCommand("shutdown now");
//}

void MainWindow::save_setting_device()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select command file"));
    m_shellcommand = Patcher::jspursing(fileName);

    save_device_settings();
}

void MainWindow::create_setting_device()
{
    create_device_settings();
}

void MainWindow::on_But_add_device_clicked()
{
    Device* dev = new Device(this);
    createRow(dev);
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




void MainWindow::save_device_settings()
{
    m_settings->beginGroup("Devices");
    m_settings->beginGroup(ui->commandtableWidget->item(1,2)->text());


    for(int i=0; i<m_shellcommand.size();i++)
    {
        if(m_shellcommand.at(i) == "SFTP")
        {
            m_settings->beginGroup("sftp");
            m_settings->setValue("LOCALPATH","./newpatch.tar.gz");
            m_settings->setValue("REMOTEPATH","/home/ivan/Remotemachine/newpatch.tar.gz");
            m_settings->endGroup();
        }
        m_settings->setValue(QString("command%1").arg(i),m_shellcommand.at(i));

    }
    m_settings->endGroup();
    m_settings->endGroup();
}

void MainWindow::create_device_settings()
{
    Device* dev;
    QString cmd;
    CommandWind* wind = new CommandWind(this);
    wind->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(wind,SIGNAL(send_command(const QString&)),this,SLOT(recive_command(const QString&)));
    connect(this,SIGNAL(get_command(QString&)),wind,SLOT(data_recived(QString&)));
    wind->show();
    auto pos = ui->commandtableWidget->currentRow();
    dev = m_device_list.at(pos);

    if(dev != nullptr)
    {
        cmd = dev->get_command_list().join("\n");
        emit get_command(cmd);
    }



}

void MainWindow::recive_command(const QString& command)
{
    Device* dev;
    auto pos = ui->commandtableWidget->currentRow();

    QStringList list = command.split("\n");
  //  Patcher::printlist(list);
    dev = m_device_list.at(pos);

    dev->set_command_list(list);
    dev->printself();

}

void MainWindow::saveSettings()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select command file"));
    m_shellcommand = Patcher::jspursing(fileName);

    m_settings->beginGroup("Devices");

    for(int i =0;i<ui->commandtableWidget->rowCount();i++)
    {
        m_settings->beginGroup(ui->commandtableWidget->item(i,2)->text());
        m_settings->setValue("Address",ui->commandtableWidget->item(i,0)->text());
        m_settings->setValue("User",ui->commandtableWidget->item(i,1)->text());
        m_settings->setValue("Device",ui->commandtableWidget->item(i,2)->text());
        m_settings->setValue("Port",ui->commandtableWidget->item(i,4)->text());
        m_settings->beginGroup("COMMAND");
        for(int j=0; j<m_shellcommand.size();j++)
        {
            if(m_shellcommand.at(j) == "sftp")
            {
                m_settings->beginGroup("SFTP");
                m_settings->setValue("LOCALPATH","./newpatch.tar.gz");
                m_settings->setValue("REMOTEPATH","/home/ivan/Remotemachine/newpatch.tar.gz");
                m_settings->endGroup();
            }
            m_settings->setValue(QString("command%1").arg(j),m_shellcommand.at(j));

        }
        m_settings->endGroup();
        m_settings->endGroup();
    }
    m_settings->endGroup();
}

void MainWindow::loadSettings()
{
    Device* device;
    m_settings->beginGroup("Devices");
    QStringList childlist = m_settings->childGroups();
    for(int i = 0;i < childlist.size();i++)
    {
        device = new Device(this);
        m_settings->beginGroup(childlist.at(i));

        device->set_addr(m_settings->value("Address").toString());
        device->set_user(m_settings->value("User").toString());
        device->set_device_name(m_settings->value("Device").toString());
        device->set_port(m_settings->value("Port").toUInt());

        m_settings->beginGroup("COMMAND");
        QStringList child = m_settings->childKeys();

        for(int j=0; j<child.size(); j++)
        {
            device->add_command(m_settings->value(QString("command%1").arg(j)).toString());
            if(m_settings->value(QString("command%1").arg(j)).toString() == "sftp")
            {
                m_settings->beginGroup("SFTP");
                device->set_sftp_local_path(m_settings->value("LOCALPATH").toString());
                device->set_sftp_remote_path(m_settings->value("REMOTEPATH").toString());
                m_settings->endGroup();
            }

        }
        m_settings->endGroup();
        m_settings->endGroup();
        createRow(device);
    }

    m_settings->endGroup();
}

void MainWindow::createRow(Device* dev)
{

    ui->commandtableWidget->insertRow(ui->commandtableWidget->rowCount());
    auto pos = (ui->commandtableWidget->rowCount() -1);

    ui->commandtableWidget->setItem(pos, 0, new QTableWidgetItem(dev->get_host()));
    ui->commandtableWidget->setItem(pos, 1, new QTableWidgetItem(dev->get_user()));
    ui->commandtableWidget->setItem(pos, 2, new QTableWidgetItem(dev->get_device_name()));
    ui->commandtableWidget->setItem(pos, 4, new QTableWidgetItem(QString::number(dev->get_port())));
    auto item = new QTableWidgetItem(tr("online?"));
    item->setForeground(Qt::green);
    ui->commandtableWidget->setItem(pos,5, item);
    ui->commandtableWidget->setItem(pos,6,new QTableWidgetItem(tr("file transfer ->")));

    QPushButton * but_save_set = new QPushButton("Save settings",this);
    QPushButton * but_create_set = new QPushButton("Create settings",this);

    connect(but_save_set,SIGNAL(clicked()),this, SLOT(save_setting_device()));
    connect(but_create_set,SIGNAL(clicked()),this, SLOT(create_setting_device()));

    ui->commandtableWidget->setCellWidget(pos, 8, but_save_set);
    ui->commandtableWidget->setCellWidget(pos, 7, but_create_set);

    m_device_list.append(dev);

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
        loadSettings();
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
        saveSettings();
    }
    else
        return;
}

void MainWindow::on_but_del_device_clicked()
{  
    auto pos = ui->commandtableWidget->currentRow();
    ui->commandtableWidget->removeRow(pos);
    m_device_list.removeAt(pos);

    if(ui->commandtableWidget->rowCount() == 0)
    {
        ui->but_del_device->setEnabled(false);
        ui->But_start_pach->setEnabled(false);
    }

    qDebug()<<"Remove: "<<pos<<"\n"<<"Count of element: "<<m_device_list.count();

    for(auto i : m_device_list)
    {
        i->printself();
    }
}


