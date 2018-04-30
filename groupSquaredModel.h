#ifndef __GROUP_SQUARED_MODEL_H__
#define __GROUP_SQUARED_MODEL_H__

#include <qabstractitemmodel.h>
#include <qlist.h>
#include "squaredRecord.h"


// <summary>
// model danych w dialogu dodawania grupowego elementów
// </summary>
class GroupSquaredModel : public QAbstractTableModel {
	Q_OBJECT

public:
	GroupSquaredModel(QObject* parent = 0);

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex& index) const;

	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	// dodawanie nowego elementu do listy
	void addSquared(float width, float height, float length, float count,bool planed = false);

	// pobiranie całej listy z elementami w celu puźniejszego wykożystania
	QList<SquaredRecord> itemsOfList() const { return m_items; }

public slots:
	void removeCheckedItems();	// usuwa wszystkie zaznaczone elementy na liście

private:
	QList<SquaredRecord> m_items;
	QStringList m_headerLabels;
};

#endif //__GROUP_SQUARED_MODEL_H__
