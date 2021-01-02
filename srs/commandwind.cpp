#include "commandwind.h"
#include "ui_commandwind.h"

CommandWind::CommandWind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommandWind)
{
    ui->setupUi(this);
}

CommandWind::~CommandWind()
{
    delete ui;
}

void CommandWind::on_buttonBox_accepted()
{
    emit send_command(ui->textEdit->toPlainText());
    close();
}

void CommandWind::on_buttonBox_rejected()
{
    close();
}

void CommandWind::data_recived(QString& command)
{
    ui->textEdit->setText(command);
}
