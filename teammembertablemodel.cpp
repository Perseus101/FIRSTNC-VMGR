#include "teammembertablemodel.h"

TeamMemberTableModel::TeamMemberTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      memberList()
{

}

TeamMemberTableModel::TeamMemberTableModel(QObject *parent, QList<TeamMember> *m_list)
    : QAbstractTableModel(parent),
      memberList(m_list)
{

}

TeamMemberTableModel::~TeamMemberTableModel()
{

}

int TeamMemberTableModel::rowCount(const QModelIndex &parent) const
{
    return memberList->size();
}

int TeamMemberTableModel::columnCount(const QModelIndex &parent) const
{
    qDebug() << "Without this debug statement, the table headers do not appear. It is unclear why.";
    return 8;
}

QVariant TeamMemberTableModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (!index.isValid() || index.row() >= memberList->size())
            return QVariant();
    {
        switch(index.column())
        {
        case 0: //First Name
            return QVariant(memberList->at(index.row()).fname);
            break;
        case 1: //Last Name
            return QVariant(memberList->at(index.row()).lname);
            break;
        case 2: //Email
            return QVariant(memberList->at(index.row()).email);
            break;
        case 3: //Phone
            return QVariant(memberList->at(index.row()).phone);
            break;
        case 4: //Title
            return QVariant(memberList->at(index.row()).title);
            break;
        case 5: //Friday
            return QVariant(memberList->at(index.row()).jobs.find(QString("Fri")).value().name);
            break;
        case 6: //Saturday
            return QVariant(memberList->at(index.row()).jobs.find(QString("Sat")).value().name);
            break;
        case 7: //Sunday
            return QVariant(memberList->at(index.row()).jobs.find(QString("Sun")).value().name);
            break;
        default:
            return QVariant();
            break;
        }
    }
        break;
    case 6:
    {
        QVariant temp;
        temp.setValue(memberList->at(index.row()));
        return temp;
    }
    default:
        return QVariant();
        break;
    }

}

QVariant TeamMemberTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
        if(orientation == Qt::Horizontal)
            switch(section)
            {
            case 0: //First Name
                return tr("First Name");
                break;
            case 1: //Last Name
                return tr("Last Name");
                break;
            case 2: //Email
                return tr("Email");
                break;
            case 3: //Phone
                return tr("Phone");
                break;
            case 4: //Title
                return tr("Badge Title");
                break;
            case 5: //Friday
                return tr("Friday Assignment");
                break;
            case 6: //Saturday
                return tr("Saturday Assignment");
                break;
            case 7: //Sunday
                return tr("Sunday Assignment");
                break;
            default:
                return QVariant();
                break;
            }
        else if(orientation == Qt::Vertical)
            return QVariant(section);
}

bool TeamMemberTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole && index.isValid())
    {
        if(memberList->size() < index.row())
            memberList->reserve(index.row());
        TeamMember temp = memberList->at(index.row());
        switch(index.column())
        {
        case 0: //First Name
            temp.fname = value.toString();
            break;
        case 1: //Last Name
            temp.lname = value.toString();
            break;
        case 2: //Email
            temp.email = value.toString();
            break;
        case 3: //Phone
            temp.phone = value.toString();
            break;
        case 4: //Title
            temp.title = value.toString();
            break;
        case 5: //Friday
            temp.jobs.find(QString("Fri")).value().name = value.toString();
            break;
        case 6: //Saturday
            temp.jobs.find(QString("Sat")).value().name = value.toString();
            break;
        case 7: //Sunday
            temp.jobs.find(QString("Sun")).value().name = value.toString();
            break;
        default:
            return false;
            break;
        }
        memberList->replace(index.row(), temp);

        emit dataChanged(index,index);
        return true;
    }
}

bool TeamMemberTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    emit beginInsertRows(QModelIndex(), position, position+rows);

    for(int i = position; i < position+rows; i++)
    {
        memberList->insert(i, TeamMember());
    }
    emit endInsertRows();
}

bool TeamMemberTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    emit beginRemoveRows(QModelIndex(), position, position+rows);

    for(int i = position+rows; i > position; i--)
    {
        memberList->removeAt(i);
    }
    emit endRemoveRows();
}

void TeamMemberTableModel::refresh()
{
    emit dataChanged(this->index(0, 0),this->index(this->rowCount(), this->columnCount()));
}

QString TeamMemberTableModel::getCSV()
{
    QString csv;

    for(int i = 0; i < rowCount(); i++)
    {
        for(int j = 0; j < columnCount(); j++)
        {
            csv.append(data(index(i, j), Qt::DisplayRole).toString() + ",");
        }
        csv.append('\n');
    }
    return csv;
}

Qt::ItemFlags TeamMemberTableModel::flags(const QModelIndex & index) const
{
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}
