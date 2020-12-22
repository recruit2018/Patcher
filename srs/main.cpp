#include <QApplication>
#include <QCryptographicHash>
#include <libssh2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "sshclient.h"
#include "sshprocess.h"
#include "sshsftp.h"
#include "sshsftpcommandsend.h"
#include "patcherfunc.h"
#include "jsoncontroller.h"
#include "mainwindow.h"


using namespace Patcher;


quint16 myport=22;

QStringList shellcommand;


int main (int argc, char** argv)
{
   QApplication a(argc, argv);

    MainWindow win;
    win.show();
    shellcommand = jspursing("./commandshell.json");


    printlist(shellcommand);

   QStringList cmdargs = QCoreApplication::arguments();

   for(auto i : cmdargs) qDebug()<<i;

   const QString source("./newpatch.tar.gz");
   QString dest("/home/ivan/Remotemachine/newpatch.tar.gz");


   SshClient* client = new SshClient("FIRST CLIENT");
   client->setPassphrase(cmdargs.at(3));

   QByteArrayList MyByteList;
   MyByteList.append("password");




    QEventLoop waitssh;
    QObject::connect(client, &SshClient::sshReady, &waitssh, &QEventLoop::quit);
    QObject::connect(client, &SshClient::sshError, &waitssh, &QEventLoop::quit);
    client->connectToHost(cmdargs.at(1),cmdargs.at(2),myport,MyByteList);
    waitssh.exec();

    if(client->sshState() != SshClient::SshState::Ready)
     {
          qDebug() << "Can't connect to connexion server";

     }else
          qDebug()  << "SSH connected";


           SshProcess *proc = client->getChannel<SshProcess>("command");

           proc->runCommand(shellcommand.at(0));

           QString localfilehash = getfilehash("./1.txt");

           QString filehash = QString::fromStdString(proc->result().toStdString());

           filehash.chop(1);



     if(filehash != localfilehash)
      {
           qDebug()<<filehash<<"   "<<localfilehash;
           QEventLoop wait;

           SshSFtp *mysftp = client->getChannel<SshSFtp>("mysftp!");
           SshSftpCommandSend *cmd = new SshSftpCommandSend(source,dest,*mysftp);
           QObject::connect(mysftp, &SshSFtp::finish, &wait, &QEventLoop::quit);

           mysftp->processCmd(cmd);
           wait.exec();
       }


    SshProcess *procc = client->getChannel<SshProcess>("command2");


    procc->runCommand(shellcommand.at(1));


    SshProcess *proccc = client->getChannel<SshProcess>("command3");

    proccc->runCommand(shellcommand.at(2));
    QString filecount = QString::fromStdString(proccc->result().toStdString());

    if(filecount.toInt() > 10)
    {

    SshProcess *procccc = client->getChannel<SshProcess>("command4");
    procccc->runCommand(shellcommand.at(3));

    }

    SshProcess *procccc = client->getChannel<SshProcess>("command5");
    procccc->runCommand("ls");


    client->~SshClient();
    sleep(10);
     client = new SshClient("FIRST CLIENT2");
    client->setPassphrase(cmdargs[3]);

    QEventLoop waitsshh;
    QObject::connect(client, &SshClient::sshReady, &waitsshh, &QEventLoop::quit);
    QObject::connect(client, &SshClient::sshError, &waitsshh, &QEventLoop::quit);
    client->connectToHost(cmdargs[1],cmdargs[2],myport,MyByteList);
    waitsshh.exec();

    if(client->sshState() != SshClient::SshState::Ready)
     {
          qDebug() << "Can't connect to connexion server";

     }else
          qDebug()  << "SSH connected";

    //SshProcess *proccccc = client->getChannel<SshProcess>("command5");
    //proccccc->runCommand("shutdown now");

    return a.exec();

}

































