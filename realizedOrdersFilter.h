#ifndef __REALIZED_ORDERS_FILTER_H__
#define __REALIZED_ORDERS_FILTER_H__

#include <qsortfilterproxymodel.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>


// <summary>
// odfiltorwanie zamówień zrealizowanych od niezrealizowanych
// </summary>
class RealizedOrdersFilter : public QSortFilterProxyModel {
	Q_OBJECT

public:
	RealizedOrdersFilter(QObject* parent = 0);

public slots:
	void setFilterEnable(int state);

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
	QSqlDatabase	m_db;				// uchwyt na połączenie z serwerem MySQL
	bool			m_filterEnable;		// czy filter jest aktywny
};

#endif //__REALIZED_ORDERS_FILTER_H__
