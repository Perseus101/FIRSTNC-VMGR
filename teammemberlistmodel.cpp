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
        return QVariant(memberList.at(index.row()).name);
    }
        break;
    case 6:
    {
        QVariant temp;
        temp.setValue(memberList.at(index.row()));
        return temp;
    }
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

    for(int i = position; i < position+rows; i++)
    {
        memberList.insert(i, TeamMember());
    }
    emit dataChanged(this->index(position),this->index(position+rows));
}

bool TeamMemberListModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), position, position+rows);

    for(int i = position+rows; i > position; i--)
    {
        memberList.removeAt(i);
    }
    emit dataChanged(this->index(position),this->index(position+rows));
}

void TeamMemberListModel::refresh()
{
    emit dataChanged(this->index(0),this->index(this->rowCount()));
}
