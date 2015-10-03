#ifndef TEAMMEMBERLISTMODEL_H
#define TEAMMEMBERLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "teammember.h"

class TeamMemberListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    TeamMemberListModel(QObject *parent = 0);

    ~TeamMemberListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
private:
    QList<TeamMember> memberList;
};


#endif // TEAMMEMBERLISTMODEL_H
