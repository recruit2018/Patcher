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
    enum Columns
    {
        Address,
        User,
        DeviceName,
        Password,
        Port,
        Stage,
        Status,
        butCRTSettings,
        butSVSettings
    };
    Q_ENUM(Columns)

    explicit DeviceModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int role) const override;
    QVariant getvalue(const int&,const int&) const;
    bool setData(const QModelIndex&, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    QVariant headerData(int, Qt::Orientation, int) const override;
    void setHeaders();
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    void setRowCount(int count) {m_device_list.clear();}
    void createRow(Device* dev);
};


