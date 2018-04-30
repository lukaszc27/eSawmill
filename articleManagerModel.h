#ifndef __ARTICLE_MANAGER_MODEL_H__
#define __ARTICLE_MANAGER_MODEL_H__

#include <qabstractitemmodel.h>
#include <qsqldatabase.h>
#include <qlist.h>
#include "articleRecord.h"

// <summary>
// model zarządzania artykułami w menadrzeże artykułów
// </summary>
class ArticleManagerModel : public QAbstractTableModel {
	Q_OBJECT

public:
	ArticleManagerModel(QObject* parent = 0);

	Qt::ItemFlags flags(const QModelIndex& index) const;

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	// wczytuje elementy do modelu z bazy danych
	void loadFromDatabase(int id = 1);
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	// zwraca tytuły kolumn
	QStringList headerList() const { return m_headerList; }

	// dodaje nowy element do bazy danych (i modelu)
	bool addItem(const QString& name,
		const QString& jm,
		const double netto,
		const double brutto,
		const double vat,
		const int magazineId,
		const float detalPrice,
		const float marza,
		const int articleId = 0);	 // wykożystywane w przypadku aktualizacji

	// usuwa artykuł z bazy danych
	bool removeArticle(const int id);

private slots:
	void removeSelectedArticle();

private:
	QStringList m_headerList;
	QList<ArticleRecord> m_items;

	QSqlDatabase m_db;
};

#endif //__ARTICLE_MANAGER_MODEL_H__
