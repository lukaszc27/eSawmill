#include "roundDelegate.h"
#include <qspinbox.h>
#include <qcheckbox.h>


RoundDelegate::RoundDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

//-----------------------------------------------
// tworzy edytor do danych w modelu
QWidget* RoundDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	if (index.column() >= 0 && index.column() <= 1)
	{
		QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
		spinBox->setMinimum(0);
		spinBox->setMaximum(10000);

		return spinBox;
	}
	else if (index.column() == 2)
	{
		QCheckBox* checkbox = new QCheckBox(parent);
		return checkbox;
	}
}

//-----------------------------------------------
// ustawia wartoœci w edytorach
void RoundDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	if (index.column() >= 0 && index.column() <= 1)
	{
		QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
		spinBox->setValue(index.data(Qt::DisplayRole).toDouble());
	}
	else if (index.column() == 2)
	{
		QCheckBox* checkbox = qobject_cast<QCheckBox*>(editor);
		checkbox->setChecked(index.data(Qt::EditRole).toBool());
	}
}

//-----------------------------------------------
// zapisuje nowe dane do modelu
void RoundDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (index.column() >= 0 && index.column() <= 1)
	{
		QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
		model->setData(index, spinBox->value());
	}
	else if (index.column() == 2)
	{
		QCheckBox* checkbox = qobject_cast<QCheckBox*>(editor);
		model->setData(index, checkbox->isChecked());
	}
}
