#include "elementsFilter.h"

ElementsFilterModel::ElementsFilterModel(QObject* parent)
	: QSortFilterProxyModel(parent)
	, m_useFilter(false)
	, m_lengthFilter(false)
{
}

//-----------------------------------------------
// odfiltrowuje wiersze na podanych kryteriach
bool ElementsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex & sourceParent) const
{
	bool filter = true;
	bool lengthFilter = true;

	if (m_useFilter)
	{
		QModelIndex widthIndex = sourceModel()->index(sourceRow, 0, sourceParent);
		QModelIndex heightIndex = sourceModel()->index(sourceRow, 1, sourceParent);

		double width = widthIndex.data(Qt::DisplayRole).toDouble();
		double height = heightIndex.data(Qt::DisplayRole).toDouble();

		if(width == m_widthValue && height == m_heightValue)
			filter = true;
		else
			filter = false;
	}

	if(m_lengthFilter)
	{
		QModelIndex lengthIndex = sourceModel()->index(sourceRow, 2, sourceParent);
		double length = lengthIndex.data(Qt::DisplayRole).toDouble();

		if(length >= m_minLength && length <= m_maxLength)
			lengthFilter = true;
		else
			lengthFilter = false;
	}

	return filter  & lengthFilter;
}
