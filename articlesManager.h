#ifndef __ARTICLE_MANAGER_H__
#define __ARTICLE_MANAGER_H__

#include <qdialog.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qsortfilterproxymodel.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>

#include "myTreeView.h"
#include "articleManagerModel.h"


// <summary>
// widget zarządzania artykułami
// </summary>
class ArticlesManager : public QDialog {
	Q_OBJECT

public:
	ArticlesManager(QWidget* parent = 0, bool selectedMode = false);

private:
	void createWidgets();
	void createShortcut();

	QPushButton* m_addButton;
	QPushButton* m_removeButton;
	QPushButton* m_editButton;
	QPushButton* m_duplicateButton;
	QLineEdit* m_filterText;
	MyTreeView* m_treeView;
	QComboBox* m_filterColumn;
	QComboBox* m_filterMagazine;

	ArticleManagerModel* m_model;
	QSortFilterProxyModel* m_proxyModel;

	QSqlDatabase m_db;	// uchyt na połączenie z bazą danych
	bool m_selectedMode;

private slots:
	void addArticle();
	void editArticle(const QModelIndex& index);	// edycja zaznaczonego artykułu
	void editArticleButtonClicked();
	void duplicateArticle();
	void filterArticle();
	void magazineActivated(int index);

signals:
	void articleSelected(int id);
};

#endif //_ARTICLE_MANAGER_H__
