#pragma once

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>
#include <QObject>
#include <QDebug>


namespace Patcher
{
#define F_COMMANDS "commandshell"



class  JsonController : public QObject
{
    Q_OBJECT

public:
   explicit JsonController(QObject * parent = nullptr);






};



class JsonCommand : public QObject
{
    Q_OBJECT
private:
    QStringList m_commands;
public:
   explicit JsonCommand(QJsonObject, QObject * parent = nullptr);
   QJsonObject toJsonObject();

   QStringList toList(const QJsonArray &array);




};


}
