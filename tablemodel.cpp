#include "tablemodel.h"

TableModel::TableModel(QObject *parent, int p_rows, int p_columns)
    : QAbstractTableModel(parent),
      m_gridData(),
      horizontalHeaders(),
      m_rows(p_rows),
      m_columns(p_columns)
{
    resize_data();
    resize_headers();
    reinitialize();
}
TableModel::~TableModel() {}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return m_rows;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return m_columns;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
        case Qt::DisplayRole:
        {
             for(int i = 0; i < m_gridData.count() ; i++)
                for(int j = 0; j < m_gridData.at(i).count(); j++)
                    if(index.row() == i && index.column() == j)
                        return QVariant(&(m_gridData.at(i).at(j)));
        }
            break;
        default:
            return QVariant();
            break;
    }
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role)
    {
        return QVariant(QString("HAY"));
        case Qt::DisplayRole:
        {
            if(orientation == Qt::Horizontal)
                for(int i = 0; i < horizontalHeaders.count() ; i++)
                    if(i == section)
                        return QVariant(&(horizontalHeaders.at(i)));
            if(orientation == Qt::Vertical)
                return section;
        }
            break;
        default:
            return QVariant();
            break;
    }
}

bool TableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        // Save value from editor to member m_gridData
        for(int i = 0; i < m_gridData.count() ; i++)
            for(int j = 0; j < m_gridData.at(i).count(); j++)
                if(index.row() == i && index.column() == j)
                {
                    m_gridData[i].replace(j, value.toInt());
                    return true;
                }
        return false;
    }
}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        //Check if data for given section header already exists
        if(orientation == Qt::Horizontal)
            for(int i = 0; i < horizontalHeaders.count() ; i++)
                if(i == section)
                    horizontalHeaders.replace(i, value.toString());
    }
    return true;
}

Qt::ItemFlags TableModel::flags(const QModelIndex & index) const
{
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

void TableModel::refresh()
{
    dataChanged(index(0, 0), index(m_rows, m_columns));
}

void TableModel::reinitialize()
{
    for (int row = 0; row < m_rows; ++row)
        for (int column = 0; column < m_columns; ++column)
            setData(index(row, column), QVariant(0));
}

void TableModel::resize_data()
{

}

void TableModel::resize_headers()
{

}


