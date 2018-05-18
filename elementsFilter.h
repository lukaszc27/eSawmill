#ifndef __ELEMENTS_FILTER_H__
#define __ELEMENTS_FILTER_H__

#include <qsortfilterproxymodel.h>
#include <QMessageBox>

// <summary>
// odfiltorwuje wiersze w pewnym zakresie
// zastosowanie g³ówne w squaredModel lub roundModel
// </summary>
class ElementsFilterModel : public QSortFilterProxyModel {
	Q_OBJECT

public:
	ElementsFilterModel(QObject* parent = 0);

public slots:
	void setWidth(double value) { m_widthValue = value; invalidateFilter(); }
	void setHeight(double value) { m_heightValue = value; invalidateFilter(); }
	void setEnable(bool enable) { m_useFilter = enable; invalidateFilter(); }
	void setLengthFilterEnable(bool enable) { m_lengthFilter = enable; invalidateFilter(); }
	void setMinLength(double value) { m_minLength = value; invalidateFilter(); }
	void setMaxLength(double value) { m_maxLength = value; invalidateFilter(); }

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
	double m_widthValue;
	double m_heightValue;
	double m_minLength;
	double m_maxLength;
	bool m_useFilter;
	bool m_lengthFilter;
};

#endif //__ELEMENTS_FILTER_H__
