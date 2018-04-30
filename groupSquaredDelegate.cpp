#include "groupSquaredDelegate.h"
#include <qspinbox.h>
#include <qcheckbox.h>

GroupSquaredDelegate::GroupSquaredDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

//-------------------------------------------------
// tworzy widgety umo�liwiaj�ce edycj� danych
// dka poszczeg�lnych kolumn
QWidget* GroupSquaredDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	if (index.column() >= 0 && index.column() <= 2)	// kolumny szeroko��, wysoko��, d�ugo��
	{
		QDoubleSpinBox* widget = new QDoubleSpinBox(parent);
		widget->setMaximum(10000);

		return widget;
	}
	if (index.column() == 3)	// kolumna ilo��
	{
		QSpinBox* widget = new QSpinBox(parent);
		widget->setMaximum(10000);

		return widget;
	}
	if (index.column() == 4)	// kolumna strugane
		return new QCheckBox(parent);

	return nullptr;
}

//-------------------------------------------------
// ustawia dane w odpowiednich edytorach
// w zale�8no�ci od kolumny
void GroupSquaredDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	if (index.column() >= 0 && index.column() <= 2) // kolumny szeroko��, wysoko��, d�ugo��
	{
		QDoubleSpinBox* widget = qobject_cast<QDoubleSpinBox*>(editor);
		widget->setValue(index.data(Qt::DisplayRole).toDouble());
	}
	if (index.column() == 3) // kolumna ilo��
	{
		QSpinBox* widget = qobject_cast<QSpinBox*>(editor);
		widget->setValue(index.data(Qt::DisplayRole).toDouble());
	}
	if (index.column() == 4)	// kolumna strugane
	{
		QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
		widget->setChecked(index.data(Qt::EditRole).toBool());
	}
}

//-------------------------------------------------
// ustawienie odpowiednich warto�ci w modelu g��wnym
void GroupSquaredDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	if (index.column() >= 0 && index.column() <= 2)	// kolumny szeroko��, wysoko��, d�ugo��
	{
		QDoubleSpinBox* widget = qobject_cast<QDoubleSpinBox*>(editor);
		model->setData(index, widget->value());
	}
	if (index.column() == 3)	// kolumna ilo��
	{
		QSpinBox* widget = qobject_cast<QSpinBox*>(editor);
		model->setData(index, widget->value());
	}
	if (index.column() == 4)	// kolumna strugane
	{
		QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
		model->setData(index, widget->isChecked());
	}
}
