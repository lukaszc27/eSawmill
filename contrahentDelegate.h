#ifndef __CONTRAHENT_DELEGATE_H__
#define __CONTRAHENT_DELEGATE_H__

#include <qstyleditemdelegate.h>

// <summary>
//  edycja danych w modelu przy użyciu MVC
// </summary>
class ContrahentDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	ContrahentDelegate(QObject* parent = 0);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif //__CONTRAHENT_DELEGATE_H__
