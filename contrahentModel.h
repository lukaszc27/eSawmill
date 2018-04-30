#ifndef __CONTRAHENT_MODEL_H__
#define __CONTRAHENT_MODEL_H__

#include <qabstractitemmodel.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qlist.h>
#include "contrahentRecord.h"


// <summary>
// Model wyświetlający dane pobrane z bazy danych na temat kontrahentów
// </summary>
class ContrahentModel : public QAbstractTableModel {
	Q_OBJECT

public:
	ContrahentModel(QObject* parent = 0, bool selectedMode = false);

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;

	Qt::ItemFlags flags(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QStringList headerLabels() const;					// zwraca listę z tytułami dla kolumn

	bool markDebator(int id);			// oznacza osobę o podanym id jako dłużnika
	bool setNormalContrahent(int id);	// zdejmuję niechlubną flagę dłużnika

public slots:
	bool loadFromDatabase();							// wczytuje dane z bazy danych MySQL
	bool addNewContrahent(const QString companyName,	// dodaje nowego kontrahenta do bazy
		const QString name,
		const QString surname,
		const QString nip,
		const QString regon,
		const QString pesel,
		const QString zipCode,
		const QString zipName,
		const QString city,
		const QString street,
		const QString houseNumber,
		const QString phone,
		const QString mail,
		const QString website);

	void removeContrahent();							// usuwa zaznaczonych kontrahentów
	void markDebatorCheckedContrahents();				// oznacza jako dłużników zaznaczonych kontrahentów
	void removeCheckedDebators();						// usuwa flagę dłuznika dla zaznaczonych osób
	void selectAll();									// zaznacza wszystkie rekordy
	void unselectAll();									// odznacza wszystkie rekordy

private:
	QList<ContrahentRecord> m_contrahents;	// lista wszystkich kontrahentów
	QStringList				m_headerLabels;	// nazwy poszczegulnych kolumn
	QSqlDatabase			m_db;			// uchwyt na aktulane połączenie z bazą danych
	bool					m_selectedMode;	// tryb wyboru kontrahenta (brak możliwości edycji danych)
};

#endif //__CONTRAHENT_MODEL_H__
