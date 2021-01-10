#pragma once

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
#include <QObject>
#include "device.h"



class DeviceModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<Device*> m_device_list;
    QSettings* m_settings;
    QStringList m_headers;
public:
    explicit DeviceModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex&, int role) const override;
    QVariant getvalue(const int&,const int&) const;
    void setvalue(const QString &, const int row, const int& column);
    bool setData(const QModelIndex&, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    QVariant headerData(int, Qt::Orientation, int) const override;
    void setHeaders();
    void createRow(Device *);
    void deleteRow(int pos);
    int row() {return m_device_list.count();}
    int column() {return m_device_list.count(); }
    void setRowCount(int count) {m_device_list.clear();}
    void delCurRow(int pos) {m_device_list.removeAt(pos);}
};


