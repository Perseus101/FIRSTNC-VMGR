#include "teammembertabledelegate.h"

TeamMemberTableDelegate::TeamMemberTableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

QWidget* TeamMemberTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void TeamMemberTableDelegate::setEditorData(QWidget *editWidget, const QModelIndex &index) const
{
    QLineEdit *editor = static_cast<QLineEdit*>(editWidget);
    editor->setText(index.model()->data(index, Qt::EditRole).toString());
}

void TeamMemberTableDelegate::setModelData(QWidget *editWidget, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *editor = static_cast<QLineEdit*>(editWidget);
    model->setData(index, editor->text(), Qt::EditRole);
}

void TeamMemberTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
