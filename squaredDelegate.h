#ifndef __SQUARED_DELEGATE_H__
#define __SQUARED_DELEGATE_H__

#include <qstyleditemdelegate.h>

// <summary>
// edytuje dane w modelu g��wnym po podw�jnym klikni�ciu w komurk�
// pomocne przy programowaniu w MVC
// </summary>
class SquaredDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	SquaredDelegate(QObject* parent = 0);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif //__SQUARED_DELEGATE_H__
