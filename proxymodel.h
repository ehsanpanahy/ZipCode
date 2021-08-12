#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStandardItem>



class ProxyModel : QSortFilterProxyModel
{
public:
    explicit ProxyModel(QObject *parent);
    QString state() const {return m_state;}


    int minimumZipcode() const;

    int maxmimumZipcode() const;

    void setState(const QString &state);

    void setMinimumZipcode(int minimumZipcode);

    void setMaxmimumZipcode(int maxmimumZipcode);

    void clearFilters();

private:
    int m_minimumZipcode;
    int m_maxmimumZipcode;
    int InvalidZipcode;
    QString m_state;
    QString m_county;

};

#endif // PROXYMODEL_H


























