#include "elementsFilter.h"

ElementsFilterModel::ElementsFilterModel(QObject* parent)
	: QSortFilterProxyModel(parent)
	, m_useFilter(false)
{
}

//-----------------------------------------------
// odfiltrowuje wiersze na podanych kryteriach
bool ElementsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex & sourceParent) const
{
	if (m_useFilter)
	{
		QModelIndex index = sourceModel()->index(sourceRow, m_filterColumn, sourceParent);
		double value = index.data().toDouble();

		if (value >= m_minValue && value <= m_maxValue)
			return true;
		else return false;
	}

	return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
