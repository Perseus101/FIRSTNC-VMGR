#ifndef TEAMMEMBERTABLEMODEL_H
#define TEAMMEMBERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "teammember.h"

class TeamMemberTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TeamMemberTableModel(QObject *parent = 0);
    TeamMemberTableModel(QObject *, QList<TeamMember> *);

    ~TeamMemberTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    Qt::ItemFlags flags(const QModelIndex & index) const;

    void refresh();
    QString getCSV();

    QList<TeamMember> *memberList;
};

#endif // TEAMMEMBERTABLEMODEL_H
