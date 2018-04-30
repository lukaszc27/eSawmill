#include "squaredDelegate.h"
#include <qspinbox.h>
#include <qcheckbox.h>

SquaredDelegate::SquaredDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

//-----------------------------------------------
// tworzy edytory do edycji danych w zale¿noœci
// od kolumn
QWidget * SquaredDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	if (index.column() >= 0 && index.column() <= 2)
	{
		QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
		spinBox->setMaximum(10000);

		return spinBox;
	}
	if (index.column() == 3)	// kolumna iloœæ
	{
		QSpinBox* spinBox = new QSpinBox(parent);
		spinBox->setMaximum(10000);

		return spinBox;
	}
	if (index.column() == 4)	// kolumna strugane
		return new QCheckBox(parent);

	return nullptr;
}

//-----------------------------------------------
// ustawia dane w edytorze
void SquaredDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	if (index.column() >= 0 && index.column() <= 2)	// kolumny szerokoœæ, wysokoœæ, d³ugoœæ
	{
		QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
		spinBox->setValue(index.data(Qt::DisplayRole).toDouble());
	}
	if (index.column() == 3)	// iloœæ
	{
		QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
		spinBox->setValue(index.data(Qt::DisplayRole).toDouble());
	}
	if (index.column() == 4)	// kolumna strugane
	{
		QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
		checkBox->setChecked(index.data(Qt::EditRole).toBool());
	}
}

//-----------------------------------------------
// ustawiamy nowe dane w modelu g³ównym
void SquaredDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	if (index.column() >= 0 && index.column() <= 2)	// kolumny szerokoœæ, wysokoœæ, d³ugoœæ
	{
		QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
		model->setData(index, spinBox->value());
	}
	if (index.column() == 3)	// iloœæ
	{
		QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
		model->setData(index, spinBox->value());
	}
	if (index.column() == 4)	// kolumna strugane
	{
		QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
		model->setData(index, checkBox->isChecked());
	}
}
