#include "devicedelegate.h"

DeviceDelegate::DeviceDelegate(QObject *parent): QStyledItemDelegate(parent)
{

}

QWidget *DeviceDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option , const QModelIndex& index) const
{
        return QStyledItemDelegate::createEditor(parent,option,index); 
}

void DeviceDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QStyledItemDelegate::setEditorData(editor,index);
}

void DeviceDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    QStyledItemDelegate::setModelData(editor,model,index);
}

void DeviceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem s = *qstyleoption_cast<const QStyleOptionViewItem*>(&option);

    if(index.column() == 5)
    {
        if(index.data().toString() == "online")
        {
            s.palette.setColor(QPalette::HighlightedText, QColor(Qt::green));
        }
        else
        {
            s.palette.setColor(QPalette::HighlightedText, QColor(Qt::red));
        }
    }
    QStyledItemDelegate::paint(painter, s, index);
}
