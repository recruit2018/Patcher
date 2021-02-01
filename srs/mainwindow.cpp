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
   //ui->tableView->setItemDelegate(new DeviceDelegate);

    m_deviceIcmp = getIcmpHandler();

    connect(m_timerStatus,SIGNAL(timeout()),this,SLOT(polling()));
   // connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),this, SLOT(somethingChanged(QTableWidgetItem*)));
    connect(this,SIGNAL(ask_status(const QString&, Device*)),m_deviceIcmp,SLOT(getStatus(const QString&, Device*)));
    connect(m_deviceIcmp,SIGNAL(send_status(bool, Device*)), this, SLOT(setStatus(bool, Device*)));

    m_deviceIcmp->moveToThread(m_thread);
    m_timerStatus->start(2000);
    m_thread->start();
}

MainWindow::~MainWindow()
{
    m_thread->quit();
    m_thread->wait();

    delete ui;
}

void MainWindow::create_device_settings()
{
    Device* dev;
    QString cmd;
    CommandWind* wind = new CommandWind(this);

    wind->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(wind,SIGNAL(send_command(const QStringList&)),this,SLOT(recive_command(const QStringList&)));
    connect(this,SIGNAL(get_command(const QStringList&)),wind,SLOT(data_recived(const QStringList&)));
    wind->show();

    auto pos = ui->tableView->currentIndex().row();
    dev =  m_model->getDevice(pos);

    if(dev != nullptr)
    {
        emit get_command(dev->get_command_list());
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

    int row = m_model->rowCount();
    m_model->insertRows(row, 1);
    // QModelIndex in = m_model->index(row, 1);
    // ui->tableView->setCurrentIndex(in);
    // ui->tableView->edit(in);
    QPushButton * but_save_set = new QPushButton("Save settings",this);
    QPushButton * but_create_set = new QPushButton("Create settings",this);
    but_save_set->setEnabled(false);
    //   connect(but_save_set,SIGNAL(clicked()),this, SLOT(save_device_settings()));
    connect(but_create_set,SIGNAL(clicked()),this, SLOT(create_device_settings()));


    ui->tableView->setIndexWidget(m_model->index(m_model->rowCount()-1 ,7),but_create_set);
    ui->tableView->setIndexWidget(m_model->index(m_model->rowCount()-1,8),but_save_set);


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
    //   connect(but_save_set,SIGNAL(clicked()),this, SLOT(save_device_settings()));
    connect(but_create_set,SIGNAL(clicked()),this, SLOT(create_device_settings()));


    ui->tableView->setIndexWidget(m_model->index(m_model->rowCount()-1 ,7),but_create_set);
    ui->tableView->setIndexWidget(m_model->index(m_model->rowCount()-1,8),but_save_set);

    if(m_model->rowCount() > 0)
    {
        ui->but_del_device->setEnabled(true);
        ui->But_start_pach->setEnabled(true);
    }
    ui->tableView->update();
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

        //ui->commandtableWidget->setRowCount(0); !!!!!!!!
       // m_model->removeRows(0, m_model->rowCount());
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
    if(!ui->tableView->currentIndex().isValid())
    {
        return;
    }

    m_mutex.lock();
    m_model->removeRows(ui->tableView->currentIndex().row(), 1, ui->tableView->currentIndex());
    m_mutex.unlock();
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
    m_mutex.lock();
    if(pos != -1)
    {
        if(val)
        {
            index = m_model->index(pos, DeviceModel::Columns::Status, QModelIndex());
            m_model->setData(index,"online",Qt::EditRole);
           // dev->set_status("online");

            //item(pos, DeviceModel::Columns::Status)->setText("online");
            //item(pos, DeviceModel::Columns::Status)->setForeground(Qt::green);
        }
        else
        {
            index = m_model->index(pos, DeviceModel::Columns::Status, QModelIndex());
            m_model->setData(index,"offline",Qt::EditRole);
        }
    }
    m_mutex.unlock();
}

void MainWindow::loadSettings()
{
    qDebug()<<"БЫЛО"<<m_model->rowCount();
    m_model->removeRows(0, m_model->rowCount());
    qDebug()<<"fdsfdsfsd"<<m_model->rowCount();
    //m_model->clearDeviceList();
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
        device->printself();
    }

    m_settings->endGroup();
}

void MainWindow::saveSettings()
{
    m_settings->clear();
    m_settings->beginGroup("Devices");
    Device* dev;
    QStringList list;
    int res=0;
    for(int i =0;i<m_model->rowCount();i++)
    {
        dev = m_model->getDevice(i);
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

void MainWindow::recive_command(const QStringList & command)
{
    Device* dev;
    auto pos = ui->tableView->currentIndex().row();
    int index=0;
    QStringList list = command;
    QStringList path;

    dev = m_model->getDevice(pos);
    index = list.indexOf(QRegExp("^sftp .+"));

    if(index >= 0)
    {
        path = list.at(index).split(' ');
        dev->set_sftp_local_path(path.at(1));
        dev->set_sftp_remote_path(path.at(2));
    }
    dev->set_command_list(list);
}

void MainWindow::polling()
{
    Device* dev;

    if (m_model->rowCount() == 0)
        return;
    for(int i=0;i<m_model->rowCount();i++)
    {
        dev = m_model->getDevice(i);
        emit ask_status(dev->get_host(), dev);
    }
}


