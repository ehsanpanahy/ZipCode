#include "tablemodel.h"

#include <QStyleOptionComboBox>

TableModel::TableModel()
{

}

const int MaxColumns = 4;

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
            index.row() < 0 || index.row() > zipcodes.count() ||
            index.column() < 0 || index.column() >= MaxColumns)
        return QVariant();

    const ZipcodeItem &item = zipcodes.at(index.row());
    if(role == Qt::SizeHintRole){
        QStyleOptionComboBox option;
        switch (index.column()) {
        case ZipCode: {

        }

        }
    }

}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if(index.isValid())
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable |
                Qt::ItemIsEnabled;

    return theFlags;
}




















