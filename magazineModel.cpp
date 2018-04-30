#include "magazineModel.h"
#include <qmessagebox.h>


MagazineModel::MagazineModel(QObject* parent)
	: QAbstractTableModel(parent)
	, m_db(QSqlDatabase::database())
{
	m_headerList << tr("Nazwa") << tr("Opis");
	loadFromDatabase();
}

//----------------------------------------
// zwraca liczbę wierszy w modelu
int MagazineModel::rowCount(const QModelIndex& index) const
{
	Q_UNUSED(index);

	return m_items.size();
}

//----------------------------------------
// wraca liczbę kolumn w modelu
int MagazineModel::columnCount(const QModelIndex & index) const
{
	Q_UNUSED(index);

	return m_headerList.size();
}

//----------------------------------------
// nadaje flagi dla poszczególnych kolumn
Qt::ItemFlags MagazineModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);
	f |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (index.column() == 0)
		f |= Qt::ItemIsUserCheckable;

	return f;
}

//----------------------------------------
// umieszcza dane w odpowiednich komórkach
QVariant MagazineModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case 0: return m_items[index.row()].name(); break;
		case 1: return m_items[index.row()].description(); break;
		}
		break;

	case Qt::UserRole:
		return m_items[index.row()].id();
		break;

	case Qt::CheckStateRole:
		if (index.column() == 0)
			return m_items[index.row()].isChecked() ? Qt::Checked : Qt::Unchecked;
		break;
	}
	return QVariant();
}

//----------------------------------------
// nadaje tytuł kolumną
QVariant MagazineModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		if (orientation == Qt::Horizontal)
			return m_headerList.at(section);
		break;
	}
	return QVariant();
}

//----------------------------------------
// zmiana wartości w modelu
bool MagazineModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	switch (role)
	{
	case Qt::CheckStateRole:
		if (index.column() == 0)
			m_items[index.row()].setChecked(value.toBool());
		break;
	}
	return true;
}

//----------------------------------------
// wczytuje listę magazynów z serwera MySQL
bool MagazineModel::loadFromDatabase()
{
	beginResetModel();
	m_items.clear();

	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery query;
		if (query.exec("SELECT * FROM artykuly_magazyn"))
		{
			while (query.next())
			{
				addItem(query.value(1).toString(), query.value(2).toString(),
					query.value(0).toInt());
			}
			return true;
		}
		else
		{
			QMessageBox::critical(0, tr("Błąd"),
				query.lastError().text());
		}
	}
	endResetModel();

	return false;
}

//----------------------------------------
// dodaje nowy rekord do modelu
void MagazineModel::addItem(const QString & name, const QString & description, int id)
{
	beginResetModel();
	MagazineRecord record(name, description, false, id);
	m_items.append(record);
	endResetModel();
}

//----------------------------------------
// dodaje nowy rekord do bazy
bool MagazineModel::addMagazine(const QString& name, const QString& description,int id)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery query;

		if (id == 0)
		{
			query.clear();
			query.prepare("INSERT INTO artykuly_magazyn (nazwa, opis) VALUES(?, ?)");
			query.bindValue(0, name);
			query.bindValue(1, description);
		}
		else
		{
			query.clear();
			query.prepare("UPDATE artykuly_magazyn SET nazwa = ?, opis = ? WHERE idMagazynu = ?");
			query.bindValue(0, name);
			query.bindValue(1, description);
			query.bindValue(2, id);
		}

		if (!query.exec())
			return false;

		loadFromDatabase();
		return true;
	}
	return false;
}

//----------------------------------------
// usuwa magazyn o podanym id
void MagazineModel::removeMagazine(int id)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery query;
		query.prepare("DELETE FROM artykuly_magazyn WHERE idMagazynu = ?");
		query.bindValue(0, id);

		if (!query.exec())
		{
			QMessageBox::critical(0, tr("Błąd"),
				query.lastError().text());

			return;
		}
	}
}

//----------------------------------------
// usuwa zaznaczone elementy
void MagazineModel::removeSelectedItems()
{
	// sprawdzamy czy użytkownik zaznaczył
	// przynajmniej jeden element
	bool bSuccess = false;
	for(int i = 0; i < m_items.size(); i++)
		if (m_items[i].isChecked())
		{
			bSuccess = true;
			break;
		}

	if (!bSuccess)
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Zanim usuniesz musisz zaznaczyć elementy do usunięcia"));

		return;
	}
	else
	{
		int iRet = QMessageBox::information(0, tr("Pytanie"),
			tr("Czy chcesz usunąć zaznaczone elementy?\n"
				"Operacja nie może być cofnięta!!!"), QMessageBox::Yes | QMessageBox::No);

		if (iRet == QMessageBox::Yes)
		{
			for(int i = 0; i < m_items.size(); i++)
				if (m_items[i].isChecked())
					removeMagazine(m_items[i].id());

			loadFromDatabase();
		}
	}
}
