#ifndef __ARTICLE_MODEL_H__
#define __ARTICLE_MODEL_H__

#include <qabstractitemmodel.h>
#include <qlist.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include "articleRecord.h"

// <summary>
// model artykułów dodanych do zamówienia/uslugi/sprzedaży
// </summary>
class ArticleModel : public QAbstractTableModel {
	Q_OBJECT

public:
	ArticleModel(QObject* parent = 0);

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;

	Qt::ItemFlags flags(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	
	// umożliwia edycje dannych w modelu
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	// dodaje nowy rekord do modelu
	void addItem(const QString& name,
		const QString& jm,
		const double netto,
		const double brutto,
		const double vat,
		const int magazineId,
		const float detalPrice,
		const float marza,
		const float count,
		const int articleId = 0);	// wykożystywane w przypadku aktualizacji

	bool loadItemsFromOrder(int orderId);		// odczytuje zapisane elementy z zamówienia
	bool loadItemsFromService(int serviceId);	// odczytuje zapisane elementy z usługi
	double priceToPay();						// zwraca całkowitą cene do zapłaty

	// zwraca listę artykułów w zamówieniu
	QList<ArticleRecord> listOfItems() const { return m_items; }
	
	// zwraca litę elementów do druku (w postaci kodu HTML)
	QString itemsToHtml();
	QStringList headerList() const { return m_headerList; }

public slots:
	void removeCheckedItem();
	void setReadOnly(bool readOnly) { m_isReadOnly = readOnly; }

private:
	QStringList m_headerList;			// tytuły kolumn
	QList<ArticleRecord> m_items;		// elementy w wierszu
	double m_priceToPay;				// całkowita cena do zapłaty
	bool m_isReadOnly;					// czy lista tylko w trybie do odczutu

	QSqlDatabase m_db;	// uchyt na połączenie z bazą danych

signals:
	void modelChanged();
};

#endif //__ARTICLE_MODEL_H__
