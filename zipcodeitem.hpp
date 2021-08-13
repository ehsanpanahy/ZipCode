#ifndef ZIPCODEITEM_HPP
#define ZIPCODEITEM_HPP

#include <QString>
struct ZipcodeItem
{
    explicit ZipcodeItem(int zipcode_=InvalidZipCode,
                         const QString &postOffice_ = QString(),
                         const QString &county_ = QString(),
                         const QString &state_ = QString())
        :zipcode(zipcode_), postOffice(postOffice_), county(county_),
          state(state_)
    {}


    bool operator<(const ZipcodeItem &other) const
    {
        return zipcode != other.zipcode ? zipcode < other.zipcode :
                                          postOffice < other.postOffice;
    }

    const static int InvalidZipCode = -1;
    int zipcode;
    QString postOffice;
    QString county;
    QString state;

};

#endif // ZIPCODEITEM_HPP



















