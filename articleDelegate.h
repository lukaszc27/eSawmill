#ifndef __ARTICLE_DELEGATE_H__
#define __ARTICLE_DELEGATE_H__

#include <qstyleditemdelegate.h>

// <summary>
// delegaty do edycji listy artykułów w zamówieniu/usłudze/sprzedaży
// </summary>
class ArticleDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	ArticleDelegate(QObject* parent = 0);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif //__ARTICLE_DELEGATE_H__
