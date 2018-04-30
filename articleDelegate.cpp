#include "articleDelegate.h"
#include <qspinbox.h>


ArticleDelegate::ArticleDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

//-------------------------------------------------------
// tworzy edytor danych dla wybranej kolumny
QWidget* ArticleDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex & index) const
{
	if (index.column() == 7)
	{
		QDoubleSpinBox* spinbox = new QDoubleSpinBox(parent);
		spinbox->setMaximum(10000);

		return spinbox;
	}
	return nullptr;
}

//-------------------------------------------------------
// ustawia wartości początkowe w edytorach
void ArticleDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	if (index.column() == 7)
	{
		QDoubleSpinBox* spinbox = qobject_cast<QDoubleSpinBox*>(editor);
		spinbox->setValue(index.data().toDouble());
	}
}

//-------------------------------------------------------
// zapisuje ustawione dane w modelu
void ArticleDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	if (index.column() == 7)
	{
		QDoubleSpinBox* spinbox = qobject_cast<QDoubleSpinBox*>(editor);
		model->setData(index, spinbox->value());
	}
}
