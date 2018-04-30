#ifndef __ELEMENTS_FILTER_H__
#define __ELEMENTS_FILTER_H__

#include <qsortfilterproxymodel.h>

// <summary>
// odfiltorwuje wiersze w pewnym zakresie
// zastosowanie g³ówne w squaredModel lub roundModel
// </summary>
class ElementsFilterModel : public QSortFilterProxyModel {
	Q_OBJECT

public:
	ElementsFilterModel(QObject* parent = 0);

public slots:
	void setMinimumValue(double value) { m_minValue = value; invalidateFilter(); }
	void setMaximumValue(double value) { m_maxValue = value; invalidateFilter(); }
	void setFilterColumn(int column) { m_filterColumn = column; invalidateFilter(); }
	void setEnable(bool enable) { m_useFilter = enable; invalidateFilter(); }

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
	int m_filterColumn;
	double m_minValue;
	double m_maxValue;
	bool m_useFilter;
};

#endif //__ELEMENTS_FILTER_H__
