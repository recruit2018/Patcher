#ifndef DEVICEDELEGATE_H
#define DEVICEDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QPainter>


class DeviceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DeviceDelegate(QObject* parent = nullptr);
    QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option, const QModelIndex&index) const;
    void setEditorData(QWidget*, const QModelIndex&) const;
    void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    void paint(QPainter* painter,const QStyleOptionViewItem& option,const QModelIndex& index) const;
};

#endif // DEVICEDELEGATE_H
