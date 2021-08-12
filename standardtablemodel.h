#ifndef STANDARDTABLEMODEL_H
#define STANDARDTABLEMODEL_H

#include <QStandardItem>



class StandardTableModel : QStandardItemModel
{
public:
    explicit StandardTableModel(QObject *parent);
    
    void initialize();
    void clear();
    void load(const QString &filename);
    void save(const QString &filename);
    
private:
    QString m_filename;
};

#endif // STANDARDTABLEMODEL_H

























