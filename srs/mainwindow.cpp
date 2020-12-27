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

    getconnectioninfo();

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
    //addDevice();
    ui->commandtableWidget->insertRow(ui->commandtableWidget->rowCount());

    tempInitRow();


    QPushButton * but_save_set = new QPushButton("Save settings",this);
    QPushButton * but_create_set = new QPushButton("Create settings",this);
    but_create_set->setEnabled(false);

    connect(but_save_set,SIGNAL(clicked()),this, SLOT(save_setting_device()));

    ui->commandtableWidget->setCellWidget(ui->commandtableWidget->rowCount()-1, 8, but_save_set);
    ui->commandtableWidget->setCellWidget(ui->commandtableWidget->rowCount()-1, 7, but_create_set);
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


int MainWindow::getconnectioninfo()
{
//    m_user = ui->commandtableWidget->item(0,1)->text();
//    m_host = ui->commandtableWidget->item(0,0)->text();
    m_device = ui->commandtableWidget->item(0,2)->text();
    return 0;
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

bool MainWindow::loadsettings()
{

    Device* device = new Device(this);
    m_settings->beginGroup("Devices");
   // Patcher::printlist(m_settings->childGroups());
    QStringList childlist = m_settings->childGroups();
    for(int i = 0;i < childlist.size();i++)
    {
        m_settings->beginGroup(childlist.at(i));
        Patcher::printlist(m_settings->childKeys());

        QStringList child = m_settings->childGroups();
        device->set_addr(m_settings->value("Address").toString());
        device->set_user(m_settings->value("User").toString());
        device->set_device_name(m_settings->value("Device").toString());
        device->set_port(m_settings->value("Port").toUInt());
        for(int j=0; j<child.size(); j++)
        {
            m_settings->beginGroup(child.at(j));
            Patcher::printlist(m_settings->childKeys());
            m_settings->endGroup();
        }

        m_settings->endGroup();
    }
    m_settings->endGroup();
    Patcher::printlist(m_settings->childGroups());

    m_devices.append(device);
}

void MainWindow::tempInitRow()
{
    ui->commandtableWidget->setItem(ui->commandtableWidget->rowCount()-1,0,new QTableWidgetItem("127.0.0.1"));
    ui->commandtableWidget->setItem(ui->commandtableWidget->rowCount()-1,1,new QTableWidgetItem("ivan"));
    ui->commandtableWidget->setItem(ui->commandtableWidget->rowCount()-1,2,new QTableWidgetItem("localhost"));
    ui->commandtableWidget->setItem(ui->commandtableWidget->rowCount()-1,4,new QTableWidgetItem("22"));
//    item = ui->commandtableWidget->item(ui->commandtableWidget->rowCount()-1,2);
//    if (item != nullptr)
}










void MainWindow::on_actionLoad_settings_triggered()
{
    loadsettings();
}

void MainWindow::on_actionSave_sattings_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Select command file"));
    m_shellcommand = Patcher::jspursing(fileName);

    m_settings->beginGroup("Devices");
    Patcher::printlist(m_settings->childGroups());

    getconnectioninfo();
    for(int i =0;i<ui->commandtableWidget->rowCount();i++)
    {
        m_settings->beginGroup(ui->commandtableWidget->item(i,2)->text());
        m_settings->setValue("Address",ui->commandtableWidget->item(i,0)->text());
        m_settings->setValue("User",ui->commandtableWidget->item(i,1)->text());
        m_settings->setValue("Device",ui->commandtableWidget->item(i,2)->text());
        m_settings->setValue("Port",ui->commandtableWidget->item(i,4)->text());

        for(int j=0; j<m_shellcommand.size();j++)
        {
            if(m_shellcommand.at(j) == "SFTP")
            {
                m_settings->beginGroup("sftp");
                m_settings->setValue("LOCALPATH","./newpatch.tar.gz");
                m_settings->setValue("REMOTEPATH","/home/ivan/Remotemachine/newpatch.tar.gz");
                m_settings->endGroup();
            }
            m_settings->setValue(QString("command%1").arg(j),m_shellcommand.at(j));

        }
        m_settings->endGroup();
    }

 m_settings->endGroup();


}
