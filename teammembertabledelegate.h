#ifndef TEAMMEMBERTABLEDELEGATE_H
#define TEAMMEMBERTABLEDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QLineEdit>

class TeamMemberTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TeamMemberTableDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // TEAMMEMBERTABLEDELEGATE_H
