#include "teammemberlistmodel.h"

TeamMemberListModel::TeamMemberListModel(QObject *parent)
    : memberList()
{

}

TeamMemberListModel::~TeamMemberListModel()
{

}

int TeamMemberListModel::rowCount(const QModelIndex &parent) const
{
    return memberList.size();
}

QVariant TeamMemberListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= memberList.size())
           return QVariant();

    switch(role)
    {
    case Qt::DisplayRole:
    {
        return QVariant(memberList.at(index.row()).getName());
    }
        break;

    default:
        return QVariant();
        break;
    }

}

QVariant TeamMemberListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

bool TeamMemberListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole && index.isValid())
    {
        memberList.replace(index.row(), qvariant_cast<TeamMember>(value));
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

bool TeamMemberListModel::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(QModelIndex(), position, position+rows);

    for(int i = position; i < rows; i++)
    {
        memberList.insert(index.row(), TeamMember());
    }
    emit dataChanged(this->index(position),this->index(position+rows));
}

bool TeamMemberListModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), position, position+rows);

    for(int i = position; i < rows; i++)
    {
        memberList.removeAt(index.row());
    }
    emit dataChanged(this->index(position),this->index(position+rows));
}
