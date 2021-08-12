#ifndef UNIQUEPROXYMODEL_H
#define UNIQUEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>
class UniqueProxymodel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit UniqueProxymodel(int column, QObject *parent = nullptr)
        :QSortFilterProxyModel(parent), Column(column){};

    void setSourceModel(QAbstractItemModel *sourceModel) override;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;
    bool filterAcceptsColumn(int source_column,
                             const QModelIndex &source_parent) const override;

private slots:
    void clearCache() {cache.clear();}

private:
    const int Column;
    mutable QSet<QString> cache;
};

#endif // UNIQUEPROXYMODEL_H






















