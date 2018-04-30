#include "articleManagerModel.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qmessagebox.h>

ArticleManagerModel::ArticleManagerModel(QObject* parent)
	: QAbstractTableModel(parent)
{
	m_headerList << tr("Nazwa") << tr("Jm") << tr("Cena netto") << tr("Cena brutto") << tr("VAT")
		<< tr("Marża") << tr("Cena detaliczna");

	m_db = QSqlDatabase::database();

	loadFromDatabase();
}

//-------------------------------------------
// flagi edycji dla poszczególnych komórek
Qt::ItemFlags ArticleManagerModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);
	f |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (index.column() == 0)
		f |= Qt::ItemIsUserCheckable;

	return f;
}

//-------------------------------------------
// zwraca liczbę wierszy w modelu
int ArticleManagerModel::rowCount(const QModelIndex & index) const
{
	Q_UNUSED(index);

	return m_items.size();
}

//-------------------------------------------
// zwraca liczbę kolumn w modelu
int ArticleManagerModel::columnCount(const QModelIndex & index) const
{
	Q_UNUSED(index);

	return m_headerList.size();
}

//-------------------------------------------
// zapisuje dane w modelu
QVariant ArticleManagerModel::data(const QModelIndex & index, int role) const
{
	int row = index.row();
	int col = index.column();

	switch (role)
	{
	case Qt::DisplayRole:
		switch (col)
		{
		case 0: return m_items[row].name(); break;
		case 1: return m_items[row].jm(); break;
		case 2: return m_items[row].priceNetto(); break;
		case 3: return m_items[row].priceBrutto(); break;
		case 4: return m_items[row].vat(); break;
		case 5: return m_items[row].marza(); break;
		case 6: return m_items[row].detalPrice(); break;
		}
		break;

	case Qt::UserRole:
		return m_items[row].id();
		break;

	case Qt::CheckStateRole:
		if (index.column() == 0)
			return m_items[row].isChecked() ? Qt::Checked : Qt::Unchecked;
		break;
	}

	return QVariant();
}

//-------------------------------------------
// nadaje tytuły kolejnym kolumną
QVariant ArticleManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	int row = 1;
	switch (role)
	{
	case Qt::DisplayRole:
		if (orientation == Qt::Horizontal)
			return m_headerList.at(section);
		else if (orientation == Qt::Vertical)
			return QString::number(row++);
		break;
	}
	return QVariant();
}

//-------------------------------------------
// wczytuje elementy z bazy danych do modelu
void ArticleManagerModel::loadFromDatabase(int id)
{
	beginResetModel();
	
	// czyścimy główna listę
	m_items.clear();

	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery q;
		q.prepare("select IdArtykulu, nazwa, cenaNetto, cenaBrutto, vat, jm, cenaDetaliczna, marza "
			"from artykuly where IdMagazynu = ?");

		q.bindValue(0, id);	// tu należy podać id magazynu

		if (q.exec())
		{
			while (q.next())
			{
				ArticleRecord record;
				record.setId(q.value(0).toInt());
				record.setName(q.value(1).toString());
				record.setNettoPrice(q.value(2).toDouble());
				record.setBruttoPrice(q.value(3).toDouble());
				record.setVat(q.value(4).toDouble());
				record.setJm(q.value(5).toString());
				record.setDetalPrie(q.value(6).toDouble());
				record.setMarza(q.value(7).toDouble());
				record.setMagazineId(1);	// to trzeba w przyszlości zmienić

				m_items.append(record);
			}
		}
		else
		{
			QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
			return;
		}
	}
	endResetModel();
}

//-------------------------------------------
// edycja wartości w modelu
bool ArticleManagerModel::setData(const QModelIndex& index, const QVariant & value, int role)
{
	QSqlQuery q;
	QString vatValue;
	switch (role)
	{
	case Qt::CheckStateRole:
		if (index.column() == 0)
			m_items[index.row()].setChecked(value.toBool());
		break;
	}

	dataChanged(index, index);
	return true;
}

//-------------------------------------------
// dodaje nowy rekord do bazy
bool ArticleManagerModel::addItem(const QString& name, 
	const QString& jm, 
	const double netto,
	const double brutto, 
	const double vat, 
	const int magazineId,
	const float detalPrice,
	const float marza,
	const int articleId)
{
	if (m_db.isValid() && m_db.isOpen())
	{
		QSqlQuery q;
		q.prepare("call artykuly_dodaj(?, ?, ?, ?, ?, ?, ?, ?, ?)");
		q.bindValue(0, magazineId);
		q.bindValue(1, name);
		q.bindValue(2, netto);
		q.bindValue(3, vat);
		q.bindValue(4, brutto);
		q.bindValue(5, jm);
		q.bindValue(6, detalPrice);
		q.bindValue(7, marza);
		q.bindValue(8, articleId);

		if (!q.exec())
		{
			QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
			return false;
		}
		loadFromDatabase();
	}
	return true;
}

//-------------------------------------------
// usuwa atyukuł z bazy danych
bool ArticleManagerModel::removeArticle(const int id)
{
	if (m_db.isValid() && m_db.isOpen())
	{
		QSqlQuery q;
		q.prepare("call artykuly_usun(?)");
		q.bindValue(0, id);

		if (q.exec())
			return true;
	}

	return false;
}

//-------------------------------------------
// usuwa zaznaczone artykuły
void ArticleManagerModel::removeSelectedArticle()
{
	// sprawdzamy czy cokolwiek zaznaczono
	bool bSuccess = false;
	for(int i = 0; i < m_items.size(); i++)
		if (m_items[i].isChecked())
		{
			bSuccess = true;
			break;
		}

	if (bSuccess)
	{
		// jeśli są zaznaczone elementy to je usuwamy
		for (int i = 0; i < m_items.size(); i++)
			if (m_items[i].isChecked())
				removeArticle(m_items[i].id());

		loadFromDatabase();
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Aby móc usunąć artykuły musisz wybrać które!"));

		return;
	}
}
