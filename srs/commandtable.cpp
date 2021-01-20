#include "commandtable.h"


CommandTable::CommandTable(QWidget * parent): QTableWidget(parent)
{
    m_thread = new QThread(this);
    m_timerStatus = new QTimer(this);
    m_settings = new QSettings(QDir::currentPath()+"/settings.ini", QSettings::IniFormat, this);

    m_deviceIcmp = getIcmpHandler();

    connect(m_timerStatus,SIGNAL(timeout()),this,SLOT(polling()));
    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),this, SLOT(somethingChanged(QTableWidgetItem*)));
    connect(this,SIGNAL(ask_status(const QString&, int)),m_deviceIcmp,SLOT(getStatus(const QString&, int)));
    connect(m_deviceIcmp,SIGNAL(send_status(bool, int)), this, SLOT(setStatus(bool, int)));

    m_deviceIcmp->moveToThread(m_thread);
    m_timerStatus->start(2000);
    m_thread->start();
}

void CommandTable::somethingChanged(QTableWidgetItem* item)
{
    if(m_device_list.isEmpty())
        return;

    auto pos = item->row();
    qDebug()<<item->row();
    Device* dev = m_device_list.at(pos);

    switch (item->column())
    {
    case Columns::Address:{dev->set_addr(item->text()); break;}
    case Columns::User:{dev->set_user(item->text());  break;}
    case Columns::DeviceName:{dev->set_device_name(item->text()); break;}
    case Columns::Port:{dev->set_port(item->text().toUInt()); break;}
    };
}

void CommandTable::setStatus(bool val, int index)
{
    QTableWidgetItem* it;
    it=item(index,Columns::Status);

    if(val){
        it->setText("online");
        it->setForeground(Qt::green);
    }
    else
    {
        it->setForeground(Qt::red);
        it->setText("offline");
    }
}

void CommandTable::createRow(Device* dev)
{
    insertRow(rowCount());
    auto pos = (rowCount() -1);
    m_device_list.append(dev);
    setItem(pos, CommandTable::Address, new QTableWidgetItem(dev->get_host()));
    setItem(pos, CommandTable::User, new QTableWidgetItem(dev->get_user()));
    setItem(pos, CommandTable::DeviceName, new QTableWidgetItem(dev->get_device_name()));
    setItem(pos, CommandTable::Port, new QTableWidgetItem(QString::number(dev->get_port())));

    auto item = new QTableWidgetItem(tr("unknown"));
    setItem(pos,CommandTable::Status, item);
    setItem(pos,CommandTable::Stage,new QTableWidgetItem(tr("wait for start")));

    QPushButton * but_save_set = new QPushButton("Save settings",this);
    QPushButton * but_create_set = new QPushButton("Create settings",this);
    but_save_set->setEnabled(false);
    connect(but_save_set,SIGNAL(clicked()),this, SLOT(save_device_settings()));
    connect(but_create_set,SIGNAL(clicked()),this, SLOT(create_device_settings()));

    setCellWidget(pos, CommandTable::butCRTSettings, but_create_set);
    setCellWidget(pos, CommandTable::butSVSettings, but_save_set);
}

void CommandTable::create_device_settings()
{
    Device* dev;
    QString cmd;
    CommandWind* wind = new CommandWind(this);

    wind->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(wind,SIGNAL(send_command(const QString&)),this,SLOT(recive_command(const QString&)));
    connect(this,SIGNAL(get_command(QString&)),wind,SLOT(data_recived(QString&)));
    wind->show();

    auto pos = currentRow();
    dev = m_device_list.at(pos);

    if(dev != nullptr)
    {
        cmd = dev->get_command_list().join("\n");
        emit get_command(cmd);
    }
}

void CommandTable::save_device_settings()
{

}

void CommandTable::addDevice()
{
    Device* dev = new Device(this);
    createRow(dev);
}

void CommandTable::saveSettings()
{
    m_settings->clear();
    m_settings->beginGroup("Devices");
    Device* dev;
    QStringList list;
    int res=0;
    for(int i =0;i<rowCount();i++)
    {
        dev = m_device_list.at(i);
        m_settings->beginGroup(QString(dev->get_device_name())+QString::number(i));
        m_settings->setValue("Address",dev->get_host());
        m_settings->setValue("User",dev->get_user());
        m_settings->setValue("Device",dev->get_device_name());
        m_settings->setValue("Port",dev->get_port());
        m_settings->beginGroup("COMMAND");

        list = dev->get_command_list();
        for(int j=0; j<list.size();j++)
        {
            res = list.at(j).indexOf(QRegExp("^sftp .+"));
            if(res >=0)
            {
                m_settings->beginGroup("SFTP");
                m_settings->setValue("LOCALPATH",dev->get_sftp_local_path());
                m_settings->setValue("REMOTEPATH",dev->get_sftp_remote_path());
                m_settings->endGroup();
            }
            m_settings->setValue(QString("command%1").arg(j),list.at(j));

        }
        m_settings->endGroup();
        m_settings->endGroup();
    }
    m_settings->endGroup();
}

void CommandTable::loadSettings()
{
    m_device_list.clear();
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
            if(m_settings->value(QString("command%1").arg(j)).toString().indexOf(QRegExp("^sftp .+")))
            {
                m_settings->beginGroup("SFTP");
                device->set_sftp_local_path(m_settings->value("LOCALPATH").toString());
                device->set_sftp_remote_path(m_settings->value("REMOTEPATH").toString());
                m_settings->endGroup();
            }
            device->add_command(m_settings->value(QString("command%1").arg(j)).toString());
        }
        m_settings->endGroup();
        m_settings->endGroup();
        createRow(device);
    }

    m_settings->endGroup();
    device->printself();
}

void CommandTable::delCurRow()
{
    auto pos = currentRow();
    removeRow(pos);
    m_device_list.removeAt(pos);

    qDebug()<<"Remove: "<<pos<<"\n"<<"Count of element: "<<m_device_list.count();

    for(auto i : m_device_list)
    {
        i->printself();
    }
}

void CommandTable::getDeviceList()
{

}

void CommandTable::recive_command(const QString& command)
{
    Device* dev;
    auto pos = currentRow();
    int index=0;
    QStringList list = command.split("\n");
    QStringList path;

    dev = m_device_list.at(pos);
    index = list.indexOf(QRegExp("^sftp .+"));

    if(index >= 0)
    {
        path = list.at(index).split(' ');
        dev->set_sftp_local_path(path.at(1));
        dev->set_sftp_remote_path(path.at(2));
    }
    //    else
    //    {
    //        dev->set_sftp_local_path("");
    //        dev->set_sftp_remote_path("");
    //    }

    dev->set_command_list(list);
}
//Bad slot. Temporary implementation
void CommandTable::polling()
{
    Device* dev;
    if (m_device_list.isEmpty())
        return;
    for(int i=0;i<m_device_list.count();i++)
    {
        dev = m_device_list.at(i);
        emit ask_status(dev->get_host(),i);
    }
}

void CommandTable::startPatching()
{

    Device * dev;
    QStringList command;

    SshSftpCommandSend* cmd;
    QByteArrayList MyByteList;
    MyByteList.append("password");

    QString localfilehash;
    QString filehash;
    QString filecount;

    for(int i= 0; i < m_device_list.count(); i++)
    {
        dev = m_device_list.at(i);
        m_client.setPassphrase(item(i,3)->text());

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
            m_proc = m_client.getChannel<SshProcess>(QString("command%1").arg(j));
            item(i,Columns::Stage)->setText(QString("Exec: command %1").arg(j));
#ifdef TASKS
            if(j == 0)
            {
                QEventLoop waitproc;
                QObject::connect(m_proc, &SshProcess::finished, &waitproc, &QEventLoop::quit);
                QObject::connect(m_proc, &SshProcess::failed, &waitproc, &QEventLoop::quit);
                m_proc->runCommand(command.at(j));
                waitproc.exec();

                filehash = QString::fromStdString(m_proc->result().toStdString());
                filehash.chop(1);
                localfilehash = Patcher::getfilehash("./1.txt");



                if(filehash != localfilehash)
                {
                    ++j;
                    qDebug()<<filehash<<"   "<<localfilehash;

                    if((command.at(j).indexOf(QRegExp("^sftp .+"))) >= 0)
                    {
                        m_sftp = m_client.getChannel<SshSFtp>("ftp");
                        cmd = new SshSftpCommandSend(dev->get_sftp_local_path(),dev->get_sftp_remote_path(),*m_sftp);
                        m_sftp->processCmd(cmd);
                    }
                }
                continue;
            }

            if( j == 2 )
            {
                QEventLoop waitproc;
                QObject::connect(m_proc, &SshProcess::finished, &waitproc, &QEventLoop::quit);
                QObject::connect(m_proc, &SshProcess::failed, &waitproc, &QEventLoop::quit);
                m_proc->runCommand(command.at(j));
                waitproc.exec();
                filecount = QString::fromStdString(m_proc->result().toStdString());
                ++j;
                if(filecount.toInt() > 10)
                {
                    qDebug()  <<command.at(j);
                    m_proc = m_client.getChannel<SshProcess>(QString("command%1").arg(j));
                    QEventLoop waitproc;
                    QObject::connect(m_proc, &SshProcess::finished, &waitproc, &QEventLoop::quit);
                    QObject::connect(m_proc, &SshProcess::failed, &waitproc, &QEventLoop::quit);
                    m_proc->runCommand(command.at(j));
                    waitproc.exec();
                }
                continue;
            }
#endif
            if((command.at(j).indexOf(QRegExp("^sftp .+"))) >= 0)
            {
                m_sftp = m_client.getChannel<SshSFtp>("ftp");
                cmd = new SshSftpCommandSend(dev->get_sftp_local_path(),dev->get_sftp_remote_path(),*m_sftp);
                m_sftp->processCmd(cmd);
                continue;
            }

            QEventLoop waitproc;
            QObject::connect(m_proc, &SshProcess::finished, &waitproc, &QEventLoop::quit);
            QObject::connect(m_proc, &SshProcess::failed, &waitproc, &QEventLoop::quit);
            m_proc->runCommand(command.at(j));
            waitproc.exec();
        }

        QEventLoop waitcli;
        m_client.disconnectFromHost();
        QObject::connect(&m_client, &SshClient::sshDisconnected, &waitcli, &QEventLoop::quit);
        QObject::connect(&m_client, &SshClient::sshError, &waitcli, &QEventLoop::quit);
        waitcli.exec();

    }
}

CommandTable::~CommandTable()
{
    m_thread->quit();
    m_thread->wait();
}

DeviceIcmp *CommandTable::getIcmpHandler()
{
//The parent is not passed because in the future the object will be moved to the thread class
#ifdef _WIN32
   return m_deviceIcmp = new DeviceIcmpWin();
#endif

#ifdef __linux__
   return m_deviceIcmp = new DeviceIcmpUnix();
#endif
}
