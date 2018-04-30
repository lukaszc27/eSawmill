#ifndef __GROUP_SQUARED_DELEGATE_H__
#define __GROUP_SQUARED_DELEGATE_H__

#include <qstyleditemdelegate.h>

// <summary>
// umo¿liwia edycjê wprowadzonych danych w dialogu dodawania grupowego elementów
// </summary>
class GroupSquaredDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	GroupSquaredDelegate(QObject* parent = 0);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif //__GROUP_SQUARED_DELEGATE_H__
