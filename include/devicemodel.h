#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractTableModel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDir>
#include <QPushButton>
#include <QDebug>
#include "device.h"
#include "commandwind.h"
#include "patcherfunc.h"

class DeviceModel : public QAbstractTableModel
{
private:
    QList<Device*> m_device_list;
    QSettings* m_settings;
    QStringList m_headers;
public:
    explicit DeviceModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;
    QVariant data(const QModelIndex&, int role) const;
    QVariant getvalue(const int&,const int&) const;
    void setvalue(const QString &, const int& column);
    bool setData(const QModelIndex&, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex&) const;
    QVariant headerData(int, Qt::Orientation, int) const;
    void setHeaders();



};

#endif // DEVICEMODEL_H
