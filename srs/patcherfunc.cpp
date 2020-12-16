#include "patcherfunc.h"


namespace Patcher
{
void printlist(const QStringList& list)
{
     for(auto i : list) qDebug()<<i;
}



QString getfilehash(const QString& path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = file.readAll();
        QByteArray hashData = QCryptographicHash::hash(fileData,QCryptographicHash::Md5);
        return QString::fromStdString(hashData.toHex().toStdString());
    }
    return QString("");
}


QStringList jspursing(const QString& filepath)
{
    QFile jfile(filepath);
    QStringList command;
     if(!jfile.open(QIODevice::ReadOnly))
     {
        qDebug()<<"File not open!";
        return QStringList();

     }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(jfile.readAll()));
     if(jsonDoc.isEmpty())
             qDebug()<<"File not QJsonDocument ";

    QJsonObject jobj = jsonDoc.object();
     if(jobj.isEmpty())
     {
          qDebug()<<"File not QJsonObject";
          return QStringList();
     }
    QJsonArray jarr = jobj.value("commandshell").toArray();
     if(jarr.isEmpty())
     {
         qDebug()<<"this not array";
         return QStringList();
     }
    foreach (QJsonValue var, jarr) {
        command.append(var.toObject()["command"].toString());
    }
        return command;
}






}
