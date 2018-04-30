#ifndef __SERVICE_MODEL_H__
#define __SERVICE_MODEL_H__

#include <qabstractitemmodel.h>
#include "roundRecord.h"
#include "squaredRecord.h"
#include "serviceRecord.h"
#include "articleRecord.h"
#include <qlist.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qdatetime.h>


// <summary>
// model wyświetlający usługi dodane do bazy
// </summary>
class ServiceModel : public QAbstractTableModel {
	Q_OBJECT

public:
	ServiceModel(QObject* parent = 0);

	Qt::ItemFlags flags(const QModelIndex& index) const;

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool loadFromDatabase();	// wczytuje dane z bazy danych
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	
	// dodaje nową usługe do listy
	void addService(const QString title,
		const QDate addDate,
		const QDate endDate,
		const bool done,
		const double rabat,
		const QString notes,
		const QList<RoundRecord> rounds,
		const QList<SquaredRecord> squards,
		const QList<ArticleRecord> articles,
		const int contrahentId = 0,
		const int id = 0,
		const int priorytet = 1,
		const int vat = 0);

	// zwraca nagłowki kolumn w modelu
	QStringList headerLists() const { return m_headerList; }

public slots:
	void removeCheckedItems();	// usuwa zaznaczone usługi z bazy
	void realiseCheckedItems();	// realizuje zaznaczone usługi
	void selectAll();			// zaznacza wszystkie usługi
	void unselectAll();			// odznacza wszystkie usługi

private:
	QStringList m_headerList;
	QList<ServiceRecord> m_items;
};

#endif //__SERVICE_MODEL_H__
