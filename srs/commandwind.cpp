#include "commandwind.h"
#include "ui_commandwind.h"


CommandWind::CommandWind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommandWind)
{
    ui->setupUi(this);
}

void CommandWind::addRow(const QString& str)
{
    QListWidgetItem *newItem = new QListWidgetItem(str);
    newItem->setFlags( Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    ui->listWidget->addItem(newItem);
}


CommandWind::~CommandWind()
{
    delete ui;
}



void CommandWind::data_recived(const QStringList &command)
{
   for(auto i:command)
   {
       addRow(i);
   }
}



void CommandWind::on_pushButton_2_clicked()
{
    delete ui->listWidget->takeItem(ui->listWidget->currentRow());
}



void CommandWind::on_pushButton_3_clicked()
{
    QStringList commands;

    for(int i=0; i < ui->listWidget->count();i++)
    {
        commands.append(ui->listWidget->item(i)->text());
    }

    commands.removeAll({});
    emit send_command(commands);
    close();

}

void CommandWind::on_pushButton_4_clicked()
{
    close();
}

void CommandWind::on_pushButton_clicked()
{
    addRow("Enter the command...");
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}
