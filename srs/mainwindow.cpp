#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model = new DeviceModel(this);
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_settings = new QSettings(QDir::currentPath()+"/settings.ini", QSettings::IniFormat, this);

    m_thread = new QThread(this);
    m_timerStatus = new QTimer(this);
    m_deviceIcmp = getIcmpHandler();
    createConnections();

    m_deviceIcmp->moveToThread(m_thread);
    m_timerStatus->start(2000);
    m_thread->start();

    setWindowTitle(tr("Patcher"));
}

void MainWindow::createConnections()
{
    connect(m_timerStatus,&QTimer::timeout,this,&MainWindow::polling);
    connect(this,&MainWindow::ask_status, m_deviceIcmp, &DeviceIcmp::getStatus);
    connect(m_deviceIcmp,SIGNAL(send_status(bool, Device*)), this, SLOT(setStatus(bool, Device*)));
}

MainWindow::~MainWindow()
{
    m_thread->quit();
    m_thread->wait();
    delete ui;
}

void MainWindow::create_device_settings()
{
    CommandWind* wind = new CommandWind(this);

    wind->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(wind, &CommandWind::send_command, this, &MainWindow::receive_command);
    connect(this, &MainWindow::get_command, wind, &CommandWind::data_recived);
    wind->show();

    auto pos = ui->tableView->currentIndex().row();
    Device* dev =  m_model->getDevice(pos);

    if(dev != nullptr)
    {
        emit get_command(dev->m_shellcommand);
    }
}

void MainWindow::on_But_start_pach_clicked()
{
    ui->But_add_device->setEnabled(false);
    ui->But_start_pach->setEnabled(false);
    ui->but_del_device->setEnabled(false);

    startPatching();

    ui->But_add_device->setEnabled(true);
    ui->But_start_pach->setEnabled(true);
    ui->but_del_device->setEnabled(true);
}


void MainWindow::save_setting_device()
{
}


void MainWindow::on_But_add_device_clicked()
{
    m_model->insertRows(m_model->rowCount(), 1);
    QPushButton* but_save_set = new QPushButton("Save settings",this);
    QPushButton* but_create_set = new QPushButton("Create settings",this);
    but_save_set->setEnabled(false);
    connect(but_create_set, &QPushButton::clicked, this, &MainWindow::create_device_settings);

    ui->tableView->setIndexWidget(m_model->index(m_model->rowCount() -1, DeviceModel::Columns::butCRTSettings), but_create_set);
    ui->tableView->setIndexWidget(m_model->index(m_model->rowCount() -1, DeviceModel::Columns::butSVSettings), but_save_set);
    ui->tableView->scrollToBottom();

    if(m_model->rowCount() > 0)
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



DeviceIcmp *MainWindow::getIcmpHandler()
{
    //The parent is not passed because in the future the object will be moved to the thread class
#ifdef _WIN32
    return m_deviceIcmp = new DeviceIcmpWin();
#endif

#ifdef __linux__
    return m_deviceIcmp = new DeviceIcmpUnix();
#endif
}

void MainWindow::startPatching()
{
    QByteArrayList MyByteList;
    MyByteList.append("password");

    for(int i= 0; i < m_model->rowCount(); i++)
    {
        Device* dev = m_model->getDevice(i);
        m_client.setPassphrase(dev->m_pass);

        QEventLoop waitssh;
        QObject::connect(&m_client, &SshClient::sshReady, &waitssh, &QEventLoop::quit);
        QObject::connect(&m_client, &SshClient::sshError, &waitssh, &QEventLoop::quit);
        m_client.connectToHost(dev->m_user, dev->m_addr, dev->m_port, MyByteList);
        waitssh.exec();

        if(m_client.sshState() != SshClient::SshState::Ready)
        {
            qDebug() << "Can't connect to connexion server";
        }else
            qDebug()  << "SSH connected";

        QStringList command = dev->m_shellcommand;
        for(int j= 0; j < command.count();j++)
        {
            m_proc = m_client.getChannel<SshProcess>(QString("command%1").arg(j));
            auto index = m_model->index(i, DeviceModel::Columns::Stage, QModelIndex());
            m_model->setData(index,QStringLiteral("Exec: command %1").arg(j),Qt::EditRole);

            if((command.at(j).indexOf(QRegExp("^sftp .+"))) >= 0)
            {
                m_sftp = m_client.getChannel<SshSFtp>("ftp");
                SshSftpCommandSend* cmd = new SshSftpCommandSend(dev->m_localfilepath,dev->m_remotefilepath,*m_sftp); //The parent is SshSFtp
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

void MainWindow::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    else
        QMainWindow::changeEvent(event);
}

void MainWindow::createRow(Device* dev)
{
    m_model->createRow(dev);
    QPushButton * but_save_set = new QPushButton("Save settings",this);
    QPushButton * but_create_set = new QPushButton("Create settings",this);
    but_save_set->setEnabled(false);
    connect(but_create_set,SIGNAL(clicked()),this, SLOT(create_device_settings()));

    ui->tableView->setIndexWidget(m_model->index(m_model->rowCount()-1, DeviceModel::Columns::butCRTSettings),but_create_set);
    ui->tableView->setIndexWidget(m_model->index(m_model->rowCount()-1, DeviceModel::Columns::butSVSettings),but_save_set);

    if(m_model->rowCount() > 0)
    {
        ui->but_del_device->setEnabled(true);
        ui->But_start_pach->setEnabled(true);
    }

    ui->tableView->update();
    ui->tableView->scrollToBottom();
}

void MainWindow::on_actionEnglish_triggered()
{
    qApp->removeTranslator(&m_tranclator);
}


void MainWindow::on_actionLoad_settings_triggered()
{
    auto ret = QMessageBox::warning(this, tr("Patcher"),
                                    tr("When downloading, all unsaved data will be lost.\n"
                                       "Do you really want to continue?"),
                                    QMessageBox::Ok | QMessageBox::Cancel);
    if(ret == QMessageBox::Ok)
    {
        loadSettings();
    }
    else
        return;

    if(m_model->rowCount() > 0)
    {
        ui->but_del_device->setEnabled(true);
        ui->But_start_pach->setEnabled(true);
    }
}

void MainWindow::on_actionSave_sattings_triggered()
{
    auto ret = QMessageBox::warning(this, tr("Patcher"),
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
    if(!ui->tableView->currentIndex().isValid())
        return;

    const bool wasBlocked = m_deviceIcmp->blockSignals(true); //Allows to discard the mutex
    m_model->removeRows(ui->tableView->currentIndex().row(), 1, ui->tableView->currentIndex());
    m_deviceIcmp->blockSignals(wasBlocked);

    if(m_model->rowCount() == 0)
    {
        ui->but_del_device->setEnabled(false);
        ui->But_start_pach->setEnabled(false);
    }
}

void MainWindow::setStatus(bool val, Device* dev)
{
    QModelIndex index;
    int pos = m_model->deviceExist(dev);

    if(pos != -1)
    {
        if(val)
        {
            index = m_model->index(pos, DeviceModel::Columns::Status, QModelIndex());
            m_model->setData(index,"online",Qt::EditRole);
        }
        else
        {
            index = m_model->index(pos, DeviceModel::Columns::Status, QModelIndex());
            m_model->setData(index,"offline",Qt::EditRole);
        }
    }
}

void MainWindow::loadSettings()
{   
    m_model->removeRows(0, m_model->rowCount());
    m_settings->beginGroup("Devices");

    QStringList childlist = m_settings->childGroups();
    for(int i = 0;i < childlist.size();i++)
    {
        Device* device = new Device(this);
        m_settings->beginGroup(childlist.at(i));

        device->m_addr = m_settings->value("Address").toString();
        device->m_user = m_settings->value("User").toString();
        device->m_device_name = m_settings->value("Device").toString();
        device->m_port = m_settings->value("Port").toUInt();

        m_settings->beginGroup("COMMAND");
        QStringList child = m_settings->childKeys();

        for(int j=0; j<child.size(); j++)
        {
            if(m_settings->value(QString("command%1").arg(j)).toString().indexOf(QRegExp("^sftp .+")))
            {
                m_settings->beginGroup("SFTP");
                device->m_localfilepath = m_settings->value("LOCALPATH").toString();
                device->m_remotefilepath = m_settings->value("REMOTEPATH").toString();
                m_settings->endGroup();
            }
            device->add_command(m_settings->value(QString("command%1").arg(j)).toString());
        }
        m_settings->endGroup();
        m_settings->endGroup();
        createRow(device);
        device->printself();
    }
    m_settings->endGroup();
}

void MainWindow::saveSettings()
{
    m_settings->clear();
    m_settings->beginGroup("Devices");

    for(int i =0;i<m_model->rowCount();i++)
    {
        Device* dev = m_model->getDevice(i);
        m_settings->beginGroup(QString(dev->m_device_name)+QString::number(i)); //Allows use of the same hostname
        m_settings->setValue("Address", dev->m_addr);
        m_settings->setValue("User", dev->m_user);
        m_settings->setValue("Device", dev->m_device_name);
        m_settings->setValue("Port", dev->m_port);
        m_settings->beginGroup("COMMAND");

        QStringList list = dev->m_shellcommand;
        for(int j=0; j<list.size();j++)
        {
            auto res = list.at(j).indexOf(QRegExp("^sftp .+"));
            if(res >=0)
            {
                m_settings->beginGroup("SFTP");
                m_settings->setValue("LOCALPATH",dev->m_localfilepath);
                m_settings->setValue("REMOTEPATH",dev->m_remotefilepath);
                m_settings->endGroup();
            }
            m_settings->setValue(QString("command%1").arg(j),list.at(j));

        }
        m_settings->endGroup();
        m_settings->endGroup();
    }
    m_settings->endGroup();
}

void MainWindow::receive_command(const QStringList& command)
{
    QStringList list = command;
    Device* dev = m_model->getDevice(ui->tableView->currentIndex().row());
    auto pos = list.indexOf(QRegExp("^sftp .+"));

    if(pos >= 0)
    {
        QStringList path = list.at(pos).split(' ');
        dev->m_localfilepath = path.at(1);
        dev->m_remotefilepath = path.at(2);
    }
    dev->m_shellcommand = list;
}

void MainWindow::polling()
{
    if (m_model->rowCount() == 0)
        return;

    for(int i= 0; i<m_model->rowCount(); i++)
    {
        Device* dev = m_model->getDevice(i);
        emit ask_status(dev->m_addr, dev);
    }
}


