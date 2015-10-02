#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QVariant>
#include <QVector>
#include <QAbstractTableModel>

#include <QDebug>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    TableModel(QObject *parent, int p_rows = 1, int p_columns = 1);
    ~TableModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex & index) const;

    void reinitialize();
    void refresh();

public slots:


signals:

private:
    int m_rows, m_columns; // Holds the number of rows and columns the table has
    QVector<QVector<float> > m_gridData; // Hold inputted data
    QVector<QString> horizontalHeaders; // Hold headers

    void addSelectedRow(int row);
    void removeSelectedRow(int row);

    void resize_data();
    void resize_headers();
};

#endif // TABLEMODEL_H
