# pragma once

#include <QStyledItemDelegate>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QPainter>

class DeviceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DeviceDelegate(QObject* parent = nullptr);
    QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option, const QModelIndex&index) const override;
    void setEditorData(QWidget*, const QModelIndex&) const override;
    void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const override;
    void paint(QPainter* painter,const QStyleOptionViewItem& option,const QModelIndex& index) const override;
};

