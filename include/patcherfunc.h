#pragma once

#include <QStringList>
#include "sshprocess.h"
#include <QFile>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

namespace Patcher
{

void printlist(const QStringList & );
QString getfilehash(const QString&);

QStringList jspursing(const QString&);





}
