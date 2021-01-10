#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    ui->commandtableWidget->startPatching();

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


