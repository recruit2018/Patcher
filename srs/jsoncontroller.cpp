#include"jsoncontroller.h"

namespace Patcher
{

JsonCommand::JsonCommand(QJsonObject jobj, QObject * parent):
    QObject(parent)
{


}

QStringList JsonCommand::toList(const QJsonArray &array)
{
    QStringList list;
    for (QJsonValue v : array) {
        list.append(v.toString());
    }
    return list;
}





}
