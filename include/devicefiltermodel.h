#pragma once

#include <QSortFilterProxyModel>

class DeviceFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
   explicit DeviceFilterModel(QObject *parent=0);

protected:
bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

};


