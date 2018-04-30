#include "ordersProxyModel.h"
#include <qmessagebox.h>
#include <qcheckbox.h>


OrdersProxyModel::OrdersProxyModel(QObject* parent)
	: QSortFilterProxyModel(parent)
	, m_filterDateColumn(0)
	, m_showOrdersAfterRealizeDays(false)
{
	setDateFilterEnable(false);
}

//----------------------------------------------------
// ustawia daty do filtra
void OrdersProxyModel::setFilterDate(const QDate firstDate, const QDate lastDate)
{
	m_firstDate = firstDate;
	m_lastDate = lastDate;
	invalidateFilter();
}

//----------------------------------------------------
// włącza lub wyłącza filtorwanie według daty
void OrdersProxyModel::setDateFilterEnable(bool enable)
{
	m_filterByDate = enable;
	invalidateFilter();
}

//----------------------------------------------------
// ustawia flagę dla filtorwania zamówień po terminie
// realizacji
void OrdersProxyModel::showOrdersAfterRealizeDays(int state)
{
	if (state == 2)
		m_showOrdersAfterRealizeDays = true;
	else if (state == 0)
		m_showOrdersAfterRealizeDays = false;

	invalidateFilter();
}

//----------------------------------------------------
// filtorwanie wierszy według daty
bool OrdersProxyModel::filterAcceptsRow(int sourceRow, 
	const QModelIndex& sourceParent) const
{
	if (m_filterByDate)
	{
		// filter według daty uruchomiony
		QModelIndex index = sourceModel()->index(sourceRow, m_filterDateColumn + 4, sourceParent);
		QVariant value = index.data();

		if (value.toDate() >= m_firstDate && value.toDate() <= m_lastDate)
			return true;
		else return false;
	}

	// wyświetlenie tylko pozycji po upłynięciu terminu realizacji
	if (m_showOrdersAfterRealizeDays)
	{
		QModelIndex index = sourceModel()->index(sourceRow, 5, sourceParent);
		QVariant value = index.data();

		if (value.toDate() <= QDate::currentDate() && !index.data(Qt::EditRole).toBool())
			return true;
		else return false;
	}

	return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
