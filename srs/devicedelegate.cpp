#include "devicedelegate.h"

DeviceDelegate::DeviceDelegate(QWidget* parent): QStyledItemDelegate(parent)
{

}

QWidget *DeviceDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option , const QModelIndex& index) const
{
    if(index.column() == 7)
    {

         QPushButton * but_save_set = new QPushButton("Save settings",parent);

        return but_save_set;

    }
    else
   {
        return QStyledItemDelegate::createEditor(parent,option,index);


    }
}

void DeviceDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
   // if(index.column() == 7)
   // {

     //   QPushButton* but = qobject_cast<QPushButton*>(editor);

  //  }
   // else
       QStyledItemDelegate::setEditorData(editor,index);
}

void DeviceDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
        QStyledItemDelegate::setModelData(editor,model,index);
}

void DeviceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column() == 7)
    {

    QPushButton* but =  qvariant_cast<QPushButton*>(index.data());


   // painter->save();
  //  painter->translate(option.rect.topLeft());
    //but->render(painter);
    painter->fillRect(option.rect,option.palette.highlight());

    }
    else
        QStyledItemDelegate::paint(painter,option,index);



}
