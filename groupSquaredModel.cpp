#include "groupSquaredModel.h"
#include <qbrush.h>
#include <qcolor.h>

GroupSquaredModel::GroupSquaredModel(QObject* parent)
	: QAbstractTableModel(parent)
{
	m_headerLabels << tr("Szerokość [cm]") << tr("Wysokość [cm]") << tr("Długość [m]") 
		<< tr("Ilość [szt]") << tr("Strugane");
}
//----------------------------------------------
// zwaraca liczbę wierszy w modelu
int GroupSquaredModel::rowCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_items.size();
}

//----------------------------------------------
// zwraca liczbę kolumn w modelu
int GroupSquaredModel::columnCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_headerLabels.size();
}

//----------------------------------------------
// wyświetla wartości danych w odpowiednich komurkach
QVariant GroupSquaredModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case 0:	 // szerokość
			return m_items[index.row()].width();
			break;

		case 1:	// wysokość
			return m_items[index.row()].height();
			break;

		case 2: // długość
			return m_items[index.row()].length();
			break;

		case 3:	// ilość
			return m_items[index.row()].count();
			break;

		case 4:	// usługa strugania
			return m_items[index.row()].isPlaned() ? tr("TAK") : tr("NIE");
			break;
		}
		break;

	case Qt::BackgroundRole:
		// jeśli element jest strugany to podświetlamy go lekko w kolorze szarym
		if (m_items[index.row()].isPlaned())
			return QVariant::fromValue(QBrush(QColor(236, 236, 236)));
		break;

	case Qt::CheckStateRole:
		// możliwość zaznaczenia oraz odznaczenia wybranej pozycji
		if (index.column() == 0)
			return m_items[index.row()].isChecked() ? Qt::Checked : Qt::Unchecked;
		break;

	case Qt::EditRole:	// tylko dla kolumny strugane
		if (index.column() == 4)
			return m_items[index.row()].isPlaned();
		break;
	}

	return QVariant();
}

//----------------------------------------------
// wyświetla tytuły kolumn
QVariant GroupSquaredModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_headerLabels[section];

	return QVariant();
}

//----------------------------------------------
// flagi informujące jak poszczególne
// korórki mają się zachowywać
Qt::ItemFlags GroupSquaredModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);
	f |= Qt::ItemIsEnabled | Qt::ItemIsEditable;
	if (index.column() == 0)
		f |= Qt::ItemIsUserCheckable;

	return f;
}

//----------------------------------------------
// nadaje nowe wartości w modelu dla odpowiednich komórek
bool GroupSquaredModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	switch (role)
	{
	case Qt::CheckStateRole:
		if (index.column() == 0)
			m_items[index.row()].setChecked(value.toBool());
		break;

	case Qt::EditRole:	// ustawienie nowych wartości przez delegata
		switch (index.column())
		{
		case 0: m_items[index.row()].setWidth(value.toDouble()); break;		// szerokość
		case 1: m_items[index.row()].setHeight(value.toDouble()); break;	// wysokość
		case 2: m_items[index.row()].setLength(value.toDouble()); break;	// długość
		case 3: m_items[index.row()].setCount(value.toInt()); break;		// ilość
		case 4: m_items[index.row()].setPlaned(value.toBool()); break;		// strugane
		}
		break;

	default:
		QAbstractTableModel::setData(index, value, role);
	}

	dataChanged(index, index);
	return true;
}

//----------------------------------------------
// dodaje nowy element do listy
void GroupSquaredModel::addSquared(float width, float height, float length, float count, bool planed)
{
	beginResetModel();
	SquaredRecord record(width, height, length, count, planed);	// tworzymy nowy tymczasowy record
	m_items.append(record);	// i dodajemy go do listy głównej
	endResetModel();
}

//----------------------------------------------
// usuwa wszystkie zaznaczone elementy na liście
void GroupSquaredModel::removeCheckedItems()
{
	QList<SquaredRecord> tmp = m_items;

	beginResetModel();
	m_items.clear();	// czyścimy całą liste a następnie uzupełniamy ją na nowo
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i].isChecked())
			continue;
		else m_items.append(tmp[i]);
	}
	endResetModel();
}
