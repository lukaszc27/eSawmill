#ifndef __MAGAZINE_MODEL_H__
#define __MAGAZINE_MODEL_H__

#include <qabstractitemmodel.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>

#include "magazineRecord.h"


// <summary>
// model danych dla menadżera magazynów
// </summary>
class MagazineModel : public QAbstractTableModel {
	Q_OBJECT

public:
	MagazineModel(QObject* parent = 0);

	int rowCount(const QModelIndex& index) const;
	int columnCount(const QModelIndex& index) const;

	// ustawia flagi dla poszczeólnych kolumn
	Qt::ItemFlags flags(const QModelIndex& index) const;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex& index,const QVariant& value,int role = Qt::EditRole);
	bool loadFromDatabase();	// wczytuje listę magazynów z serwera MySQL

	// dodaje nowy rekord do modelu
	void addItem(const QString& name, const QString& descrption, int id);

	// dodaje nowy rekord do bazy
	bool addMagazine(const QString& name, const QString& description,int id = 0);

	// usuwa magazyn o podanym id
	void removeMagazine(int id);

public slots:
	void removeSelectedItems();	// usuwa zaznaczone elementy

private:
	QSqlDatabase m_db;

	QStringList m_headerList;
	QList<MagazineRecord> m_items;
};

#endif //__MAGAZINE_MODEL_H__
