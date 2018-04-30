#ifndef __ROUND_DELEGATE_H__
#define __ROUND_DELEGATE_H__

#include <qstyleditemdelegate.h>

class RoundDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	RoundDelegate(QObject* parent = 0);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif //__ROUND_DELEGATE_H__
