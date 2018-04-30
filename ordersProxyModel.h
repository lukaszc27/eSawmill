#ifndef __ORDERS_PROXY_MODEL_H__
#define __ORDERS_PROXY_MODEL_H__

#include <qsortfilterproxymodel.h>
#include <qdatetime.h>

// <summary>
// filtorwanie wierszy według podanych kryteriów
// </summary>
class OrdersProxyModel : public QSortFilterProxyModel {
	Q_OBJECT

public:
	OrdersProxyModel(QObject* parent);

	void setFilterDate(const QDate firstDate, const QDate lastDate);
	void setDateFilterEnable(bool enable);
	void setDateFilterColumn(int column) {
		m_filterDateColumn = column;
		invalidateFilter();
	}

public slots:
	// wyświetla tylko zamówienia po upłyniećiu terminu realizacji (czerwone)
	void showOrdersAfterRealizeDays(int state);

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
	QDate			m_firstDate;					// minimalna data
	QDate			m_lastDate;						// maksymalna data
	bool			m_filterByDate;					// czy filtrować rekordy po dacie
	bool			m_showOrdersAfterRealizeDays;	// czy wyświetlać zamówienia po upłynięciu terminu realizacji
	int				m_filterDateColumn;				// według której kolumny fiktorwać dane
};

#endif //__ORDERS_PROXY_MODEL_H__
