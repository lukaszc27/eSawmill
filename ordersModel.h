#ifndef __ORDERS_MODEL_H__
#define __ORDERS_MODEL_H__

#include <qabstractitemmodel.h>
#include <qlist.h>
#include "orderRecord.h"
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qdatetime.h>
#include <qlist.h>
#include "squaredRecord.h"
#include "articleRecord.h"

// <summary>
// model danych wykorzystywany przez menadżera zamówień
// </summary>
class OrdersModel : public QAbstractTableModel {
	Q_OBJECT

public:
	OrdersModel(QObject* parent = 0);

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;

	Qt::ItemFlags flags(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;	// ndaje tytuły kolumną

	// ustawia nowe dane w głównej liście
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	// zwraca listę tytułową
	QStringList headerLabels() const { return m_headerLabels; }

	bool loadFromDatabase();				// wczytuje dane zapisane w bazie danych MySQL
	
	// dodaje zamówienie do bazy danych MySQL i zwraca jego ID
	int addOrderToDatabase(const int orderId,
		const int contrahentId,
		const QDate addDate,
		const QDate endDate,
		const QString title,						// tytuł
		const float saveLength,						// zapas długości (naddatek)
		const bool done,							// czy zrealizowane
		const int rabat,							// rabat,
		const QString notes,						// notatki
		const int priotytet,
		const QString vatText,						// stawka vat
		const double price,
		const QList<SquaredRecord> listOfItems,		// lista elementów do dodania w zamówieniu
		const QList<ArticleRecord> articlesItems);	// lista artykułów dodanych do zamówienia

	bool realizeOrder(int id);	// ustawia flagę zrealizowania

public slots:
	void removeCheckedOrders();			// usuwa zaznaczone zamówienia
	void realizeCheckedOrders();		// realizuje zaznaczone zamowienia
	void selectAllOrders();				// zaznacza wszystkie zamówienia
	void unselectAllOrders();			// odznacza wszystkie zamówienia

private:
	QSqlDatabase		m_db;				// uchwyt na połączenie z bazą danych
	QList<OrderRecord>	m_orders;			// lista z zamówieniami
	QStringList			m_headerLabels;		// nagłówki każdej z kolumn
};

#endif //__ORDERS_MODEL_H__
