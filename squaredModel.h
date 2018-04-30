#ifndef __SQUARED_MODEL_H__
#define __SQUARED_MODEL_H__

#include <qabstractitemmodel.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qmessagebox.h>
#include "squaredRecord.h"

class SquaredModel : public QAbstractTableModel {
	Q_OBJECT

public:
	SquaredModel(QObject* parent = 0);

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;

	Qt::ItemFlags flags(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::EditRole) const;

	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	// dodaje nowy element do listy
	void addItem(float width, float height, float length, float count, bool planed = false, bool checked = false);
	void addItem(SquaredRecord record);
	void readOnly(bool readOnly) { m_readOnly = readOnly; }	// ustawia możliwość tylko od odczytu (po zrealizowaniu zamówienia)

	double sumOfMetric();								// zwraca całkowitą ilość m3 w zamówieniu
	double planedOfMetric();							// ilość m3 drewna struganego
	double price();										// wylicza całość kosztów do zapłaty (bez rabatu)

	int rabat() const { return m_rabat; }				// zwraca wielkość rabatu

	bool exportToXML(const QString& fileName);			// zapisuje dane w pliku XML
	bool importFromXML(const QString& fileName);		// odczytujemy dane zapisane w pliku XML
	bool loadFromOrder(const int id);					// wczytuje dane z zamówienia po ID
	bool loadFromService(const int id);					// wczytuje dane z usługi po ID
	

	QString itemsToHtml();	// zapisuje listę elementów razem z podliczeniem kosztów

	QList<SquaredRecord> listOfItems() const { return m_items; }// zwraca główną listę

public slots:
	void removeCheckedItems();				// usuwa zaznaczone elementy z listy po odczymaniu zgody
	void selectAll();						// zaznacza wszystkie elementy
	void unselectAll();						// odznacza wszystkie elementy

	// ustawia wielkość rabatu
	void setRabat(int rabat) { 
		m_rabat = rabat; 
		emit modelChanged();
	};

	// ustawia cene dla kantówek (bez usługi strugania)
	void setPrice(double price) {
		m_price = price;
		emit modelChanged();
	}

	// aktywuje lub dezaktywuje doliczenie VAT-u do kosztorysku
	void setVatEnable(bool enable) { 
		m_vatEnable = enable; 
		emit modelChanged();
	}
	
	// ustawia wartość stawki VAT
	void setVatValue(int value) { 
		m_vat = value; 
		emit modelChanged();
	}

	bool vatEnabled() const { return m_vatEnable; }
	int vatValue() const { return m_vat; }

private:
	QSqlDatabase			m_db;				// uchwyt na aktualne połączenie z bazą danych
	QStringList				m_headerLabels;		// lista z tytułami nagłówków
	QList<SquaredRecord>	m_items;			// lista elementów
	bool					m_readOnly;			// flaga tylko do odczytu
	bool					m_vatEnable;		// czy doliczać wartość VAT
	int						m_vat;				// wartość naliczanego VAT-u
	int						m_rabat;			// wielkość rabatu
	double					m_price;			// cena kantówek (bez strugania)

signals:
	void modelChanged();	// emitowany jeś6li w modelu zaszły jakieś zmainy
};

#endif //__SQUARED_MODEL_H__
