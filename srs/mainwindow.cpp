#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_shellcommand = Patcher::jspursing("./commandshell.json");
    MyByteList.append("password");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    m_user = ui->commandtableWidget->item(0,2)->text();
    m_host = ui->commandtableWidget->item(0,0)->text();
    m_pass = ui->commandtableWidget->item(0,3)->text();


qDebug() << "Can't connect to connexion server";

    m_client.setPassphrase(m_pass);
qDebug() << "Can't connect to connexion server";
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


             //    client->~SshClient();
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

void MainWindow::on_pushButton_2_clicked()
{

    ui->commandtableWidget->insertRow(ui->commandtableWidget->rowCount());

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
