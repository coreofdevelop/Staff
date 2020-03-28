#ifndef FILTERPROXYMODEL_H
#define FILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>

class FilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FilterProxyModel(QObject *parent = NULL);
    void setFilter(int monthBefore, int filterColum);
    void resetFilter();

private:
    int _filterColum = 0;
    int _monthBefore = 0;
    //QDate _dateBefore;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // FILTERPROXYMODEL_H
