#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MyByteList.append("password");
    m_settings = new QSettings(QDir::currentPath()+"/settings.ini", QSettings::IniFormat, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_But_start_pach_clicked()
{

    m_client.setPassphrase(m_pass);
    QEventLoop waitssh;
    QObject::connect(&m_client, &SshClient::sshReady, &waitssh, &QEventLoop::quit);
    QObject::connect(&m_client, &SshClient::sshError, &waitssh, &QEventLoop::quit);
    m_client.connectToHost(m_user,m_host,22,MyByteList);
    waitssh.exec();

    if(m_client.sshState() != SshClient::SshState::Ready)
     {
          qDebug() << "Can't connect to connexion server";
     }else
          qDebug()  << "SSH connected";


               SshProcess *proc = m_client.getChannel<SshProcess>("command");

               proc->runCommand(m_shellcommand.at(0));

               QString localfilehash = Patcher::getfilehash("./1.txt");

               QString filehash = QString::fromStdString(proc->result().toStdString());

               filehash.chop(1);

                  const QString source("./newpatch.tar.gz");
                  QString dest("/home/ivan/Remotemachine/newpatch.tar.gz");

         if(filehash != localfilehash)
          {
               qDebug()<<filehash<<"   "<<localfilehash;
               QEventLoop wait;

               SshSFtp *mysftp = m_client.getChannel<SshSFtp>("mysftp!");
               SshSftpCommandSend *cmd = new SshSftpCommandSend(source,dest,*mysftp);
               QObject::connect(mysftp, &SshSFtp::finish, &wait, &QEventLoop::quit);

               mysftp->processCmd(cmd);
               wait.exec();
           }

             SshProcess *procc = m_client.getChannel<SshProcess>("command2");


             procc->runCommand(m_shellcommand.at(1));


             SshProcess *proccc = m_client.getChannel<SshProcess>("command3");

             proccc->runCommand(m_shellcommand.at(2));
             QString filecount = QString::fromStdString(proccc->result().toStdString());

             if(filecount.toInt() > 10)
             {

             SshProcess *procccc = m_client.getChannel<SshProcess>("command4");
             procccc->runCommand(m_shellcommand.at(3));

             }

             SshProcess *procccc = m_client.getChannel<SshProcess>("command5");
             procccc->runCommand("ls");


             m_client.disconnectFromHost();
             //    sleep(10);
             //     client = new SshClient("FIRST CLIENT2");
             //    client->setPassphrase(cmdargs[3]);

             //    QEventLoop waitsshh;
             //    QObject::connect(client, &SshClient::sshReady, &waitsshh, &QEventLoop::quit);
             //    QObject::connect(client, &SshClient::sshError, &waitsshh, &QEventLoop::quit);
             //    client->connectToHost(cmdargs[1],cmdargs[2],myport,MyByteList);
             //    waitsshh.exec();

             //    if(client->sshState() != SshClient::SshState::Ready)
             //     {
             //          qDebug() << "Can't connect to connexion server";

             //     }else
             //          qDebug()  << "SSH connected";

             //    //SshProcess *proccccc = client->getChannel<SshProcess>("command5");
             //    //proccccc->runCommand("shutdown now");
}

void MainWindow::save_setting_device()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select command file"));
    m_shellcommand = Patcher::jspursing(fileName);

    save_device_settings();
}

void MainWindow::on_But_add_device_clicked()
{
    Device* dev = new Device(this);
    createRow(dev);
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




bool MainWindow::save_device_settings()
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

bool MainWindow::loadSettings()
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
      //  m_device_list.append(device);
    }
//    for(auto i : m_device_list)
//    {
//        i->printself();

//    }
    m_settings->endGroup();
}

void MainWindow::createRow(Device* dev)
{

    ui->commandtableWidget->insertRow(ui->commandtableWidget->rowCount());

    ui->commandtableWidget->setItem(ui->commandtableWidget->rowCount()-1,0,new QTableWidgetItem(dev->get_host()));
    ui->commandtableWidget->setItem(ui->commandtableWidget->rowCount()-1,1,new QTableWidgetItem(dev->get_user()));
    ui->commandtableWidget->setItem(ui->commandtableWidget->rowCount()-1,2,new QTableWidgetItem(dev->get_device_name()));
    ui->commandtableWidget->setItem(ui->commandtableWidget->rowCount()-1,4,new QTableWidgetItem(QString::number(dev->get_port())));

    QPushButton * but_save_set = new QPushButton("Save settings",this);
    QPushButton * but_create_set = new QPushButton("Create settings",this);
    but_create_set->setEnabled(false);

    connect(but_save_set,SIGNAL(clicked()),this, SLOT(save_setting_device()));

    ui->commandtableWidget->setCellWidget(ui->commandtableWidget->rowCount()-1, 8, but_save_set);
    ui->commandtableWidget->setCellWidget(ui->commandtableWidget->rowCount()-1, 7, but_create_set);

    m_device_list.append(dev);

}




void MainWindow::on_actionLoad_settings_triggered()
{
    int ret = QMessageBox::warning(this, tr("Patcher"),
                                   tr("When downloading, all unsaved data will be lost.\n"
                                      "Do you really want to continue?"),
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok)
    {
        ui->commandtableWidget->setRowCount(0);
        loadSettings();
    }
    else
        return;
}

void MainWindow::on_actionSave_sattings_triggered()
{
    int ret = QMessageBox::warning(this, tr("Patcher"),
                                   tr("When saving, the previous settings will be permanently changed.\n"
                                      "Do you really want to continue?"),
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok)
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

    qDebug()<<"remove :"<<pos;
    for(auto i : m_device_list)
    {
        i->printself();

    }
}
