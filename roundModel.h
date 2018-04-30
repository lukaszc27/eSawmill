#ifndef __ROUND_MODEL_H__
#define __ROUND_MODEL_H__

#include <qabstractitemmodel.h>
#include <qlist.h>
#include "roundRecord.h"

// <summary>
// Model danych przedstawiających kłody
// </summary>
class RoundModel : public QAbstractTableModel {
	Q_OBJECT

public:
	RoundModel(QObject* parent = 0);

	enum Columns {
		Diameter = 0,	// średnica
		Length,			// długość
		Eaves,			// czy kantowane (przecinane 2 razy)
		Kubatura		// ilość kubików w kłodzie
	};

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;
	
	int vat() const { return m_vat; }
	double rabat() const { return m_rabat; }

	Qt::ItemFlags flags(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	// dodaje nowy wiersz (nową kłode do listy)
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	bool addItem(double diameter, double length, bool eaves = false, bool checked = false);
	bool exportToXML(const QString& fileName);	// zapisuje listę do pliku
	bool importFromXML(const QString fileName);	// odczytuje listę z pliku
	bool loadFromService(const int id);			// wczytuje dane z usługi
	bool isVatEnable() const { return m_vatEnabled; }

	double sumOfMetric();		// oblicza ilość kubików dodanego drewna
	double sumOfEavesMetric();	// oblicza ilość drewna kantowanego
	double priceOfMetric();		// oblicza cenę do zapłaty za drewno

	// ustawia możliwość tylko do odczytu
	void setReadOnly(bool readOnly) { m_readOnly = readOnly; }
	
	QString itemsToHtml();	// przepusje listę do formatu HTML
	
	// zwracamy liste elementów
	QList<RoundRecord> listOfItems() const { return m_items; }


public slots:
	void selectAll();			// zaznacza wszystkie elemnty na liście
	void unselectAll();			// odznacza wszystkie elemeny z listy
	void removeCheckedItems();	// usuwa z listy zaznaczone elementy
	
	// ustawia wielkość rabatu dla ceny
	void setRabat(double rabat) { 
		m_rabat = rabat;
		emit modelChanged();
	}

	// ustawia VAT dla uslugi
	void setVat(int vat) {
		m_vat = vat;
		emit modelChanged();
	}

	// aktywuje lub dezaktywuje doliczanie VAT-u
	void setVatEnabled(bool enabled) {
		m_vatEnabled = enabled;
		emit modelChanged();
	}

private:
	QStringList m_headerLabels;
	QList<RoundRecord> m_items;
	bool m_readOnly;
	bool m_vatEnabled;
	int m_vat;
	double m_rabat;

signals:
	void modelChanged();	// emitowany jeżeli zaszły jakieś zmiany w modelu
};

#endif //__ROUND_MODEL_H__
