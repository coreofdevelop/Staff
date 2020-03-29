#include "filterproxymodel.h"

FilterProxyModel::FilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

void FilterProxyModel::setFilter(int monthBefore, int filterColum)
{
    _filterColum = filterColum;
    _monthBefore = monthBefore;
    invalidateFilter();
}

void FilterProxyModel::resetFilter()
{
    _filterColum = 0;
    invalidateFilter();
}

bool FilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // Перегруженная функция для создания собственого

    // Получаем индекс для нашей перегруженной функции из модели источника
    QModelIndex index = sourceModel()->index(source_row, _filterColum, source_parent);
    // Получаем значение из нашего только что созданого индекса
    QVariant value = sourceModel()->data(index, Qt::DisplayRole);
    // Проверяем на валидность это значение
    if (value.isValid()){
        // Приводим значение к типу QString
        QDate dateValue = value.toDate();
        // Если не один из фильтров не выбран считаем, что фильтр выключен
        if (_filterColum != 3 && _filterColum != 4 && _filterColum != 5 && _filterColum != 6)
            return true;
        // В зависимости от предустановленого месяца сравниваются даты
        switch (_monthBefore) {
        case 0:
            return 0 >= QDate::currentDate().daysTo(dateValue);
        case 1:
            return 30 > QDate::currentDate().daysTo(dateValue);
        case 2:
            return 60 > QDate::currentDate().daysTo(dateValue);
        case 3:
            return 90 > QDate::currentDate().daysTo(dateValue);
        case 4:
            return 120 > QDate::currentDate().daysTo(dateValue);
        case 5:
            return 150 > QDate::currentDate().daysTo(dateValue);
        case 6:
            return 180 > QDate::currentDate().daysTo(dateValue);
        }
    }
    // Если дата установлена не моделью, используем стандартный фильтр
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

}
