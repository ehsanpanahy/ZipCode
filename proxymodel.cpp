#include "proxymodel.hpp"

#include <global.hpp>

ProxyModel::ProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent)
{
    m_minimumZipcode = m_maxmimumZipcode = InvalidZipcode;
}

int ProxyModel::minimumZipcode() const
{
    return m_minimumZipcode;
}

int ProxyModel::maxmimumZipcode() const
{
    return m_maxmimumZipcode;
}

void ProxyModel::setState(const QString &state)
{
    if (m_state != state)
    {
        m_state = state;
        invalidateFilter();
    }

}

void ProxyModel::setMinimumZipcode(int minimumZipcode)
{
    if(m_minimumZipcode != minimumZipcode)
    {
        m_minimumZipcode = minimumZipcode;
        invalidateFilter();
    }

}

void ProxyModel::setMaxmimumZipcode(int maxmimumZipcode)
{
    if (m_maxmimumZipcode != maxmimumZipcode)
    {
        m_maxmimumZipcode = maxmimumZipcode;
        invalidateFilter();

    }

}

void ProxyModel::clearFilters()
{
    m_minimumZipcode = m_maxmimumZipcode = InvalidZipcode;
    m_county.clear();
    m_state.clear();
    invalidateFilter();

}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (m_minimumZipcode != InvalidZipcode ||
            m_maxmimumZipcode !=InvalidZipcode)
    {
        QModelIndex index = sourceModel()->index(source_row, Zipcode, source_parent);

        if(m_minimumZipcode != InvalidZipcode &&
                sourceModel()->data(index).toInt() < m_minimumZipcode)
            return false;
        if(m_maxmimumZipcode != InvalidZipcode &&
                sourceModel()->data(index).toInt() > m_maxmimumZipcode)
            return false;
    }

    if(!m_county.isEmpty())
    {
        QModelIndex index = sourceModel()->index(source_row, County,
                                                 source_parent);
        if(m_county != sourceModel()->data(index).toString())
            return false;
    }

    if(!m_state.isEmpty())
    {
        QModelIndex index = sourceModel()->index(source_row, State,
        source_parent);
        if (m_state != sourceModel()->data(index).toString())
            return false;
    }

    return true;

}















