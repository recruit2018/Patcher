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
}

MainWindow::~MainWindow()
{
    delete ui;
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
        dev = m_device_list.at(i);
        m_settings->beginGroup(dev->get_device_name());
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

void MainWindow::loadSettings()
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
        m_model->createRow(device);
    }

    m_settings->endGroup();
    device->printself();
}

void MainWindow::create_setting_device()
{
//    CommandWind* wind = new CommandWind(this);
//    wind->setAttribute(Qt::WA_DeleteOnClose, true);

//    connect(wind,SIGNAL(send_command(const QString&)),this,SLOT(recive_command(const QString&)));
//    connect(this,SIGNAL(get_command(QString&)),wind,SLOT(data_recived(QString&)));
//    wind->show();

//    create_device_settings();
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
    auto pos = ui->tableView->currentIndex().row();
    qDebug()<<pos;
    dev = m_device_list.at(pos);

    if(dev != nullptr)
    {
        cmd = dev->get_command_list().join("\n");
        emit get_command(cmd);
    }
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


void MainWindow::on_but_del_device_clicked()
{

    m_model->removeRows(ui->tableView->currentIndex().row(), 1, ui->tableView->currentIndex());

    if(m_model->rowCount() == 0)
    {
        ui->but_del_device->setEnabled(false);
        ui->But_start_pach->setEnabled(false);
    }
}

void MainWindow::on_But_start_pach_clicked()
{



}



void MainWindow::save_setting_device()
{


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
        m_model->setRowCount(0);
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



void MainWindow::recive_command(const QString& command)
{
    Device* dev;
    auto pos = ui->tableView->currentIndex().row();
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
