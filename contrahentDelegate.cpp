#include "contrahentDelegate.h"
#include <qlineedit.h>


ContrahentDelegate::ContrahentDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

//-----------------------------------------------------
// tworzenie edytora dla danych w odpowiednich komurkach
QWidget * ContrahentDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	if (!index.isValid())
		return nullptr;

	return new QLineEdit(parent);	// w każdej kolumnie widget QLineEdit
}

//-----------------------------------------------------
// ustawienie w edytorze wartości podstawowej (wstępnej)
void ContrahentDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	QLineEdit* edit = qobject_cast<QLineEdit*>(editor);
	edit->setText(index.data(Qt::DisplayRole).toString());
}

//-----------------------------------------------------
// zapisanie wprowadzonych danych w modelu danych
void ContrahentDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	QLineEdit* edit = qobject_cast<QLineEdit*>(editor);
	model->setData(index, edit->text().toUpper(), Qt::EditRole);
}