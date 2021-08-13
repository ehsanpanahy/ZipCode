#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "zipcodeitem.hpp"

class TableModel : public QAbstractTableModel
{
public:
    TableModel();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;


private:
    QList<ZipcodeItem> zipcodes;
};

#endif // TABLEMODEL_H












