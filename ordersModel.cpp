#include "ordersModel.h"
#include <qmessagebox.h>



OrdersModel::OrdersModel(QObject* parent)
	: QAbstractTableModel(parent)
{
	m_db = QSqlDatabase::database();	// pobieramy uchwyt na połączenie

	m_headerLabels << tr("Tytuł zamówienia") << tr("Firma") << tr("Imię") << tr("Nazwisko") << tr("Data przyjęcia")
		<< tr("Termin realizacji") << tr("Priotytet");

	loadFromDatabase();	// wczytujemy dane zapisane w bazie MySQL
}

//--------------------------------------------------
// zwraca liczbę wierszy w modelu
int OrdersModel::rowCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_orders.size();
}

//--------------------------------------------------
// zwraca liczbę kolumn w modelu
int OrdersModel::columnCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_headerLabels.size();
}

//--------------------------------------------------
// flagi dla poszczególnych elementów modelu
Qt::ItemFlags OrdersModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);
	f |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (index.column() == 0)
		f |= Qt::ItemIsUserCheckable;

	return f;
}

//--------------------------------------------------
// uzupełnia odpowiednie komórki danymi
QVariant OrdersModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case 0: return m_orders[index.row()].title(); break;
		case 1: return m_orders[index.row()].companyName(); break;
		case 2: return m_orders[index.row()].name(); break;
		case 3: return m_orders[index.row()].surname(); break;
		case 4: return m_orders[index.row()].addDate(); break;
		case 5: return m_orders[index.row()].endDate(); break;
		case 6:
			switch (m_orders[index.row()].priotytet())
			{
			case 0: return tr("NISKI"); break;
			case 1: return tr("NORMALNY"); break;
			case 2: return tr("WYSOKI"); break;
			};
			break;
		}
		break;

	case Qt::UserRole:	// zwracamy id zamówienia
		return QVariant::fromValue(m_orders[index.row()].id());
		break;

	case Qt::EditRole:	// czy zamówienie jest zrealizowane
		return m_orders[index.row()].isDone();
		break;

	case Qt::CheckStateRole:
		// możliwość zaznaczenia tylko w pierwszej kolumnie
		if (index.column() == 0)
			return m_orders[index.row()].isChecked() ? Qt::Checked : Qt::Unchecked;
		break;

	case Qt::TextColorRole:
		// jeśli zamówienie zrealizowane to wyświetlamy w kolorze lekko szarym
		if (m_orders[index.row()].isDone())
			return QVariant::fromValue(QBrush(Qt::gray));
		break;

	case Qt::BackgroundRole:
		// jeśli termin realizacji minoł to podświetlamy zamówienie na czerwono
		if (m_orders[index.row()].endDate() <= QDate::currentDate() && !m_orders[index.row()].isDone())
		{
			// po terminie realizacji, podświetlenie na czerwono
			return QVariant::fromValue(QBrush(Qt::red));
		}
		break;
	}

	return QVariant();
}

//--------------------------------------------------
// nadaje tytuły kolumną
QVariant OrdersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_headerLabels.at(section);

	return QVariant();
}

//--------------------------------------------------
// ustawia nowe dane w głównej liście z elementami
bool OrdersModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	switch (role)
	{
	case Qt::CheckStateRole:
		if (index.column() == 0)
			m_orders[index.row()].setChecked(value.toBool());
		break;

	default:
		return QAbstractTableModel::setData(index, value, role);
	}

	return true;
}

//--------------------------------------------------
// wczytuje dane zapisane w bazie danych MySQL
bool OrdersModel::loadFromDatabase()
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery query;
		if (query.exec("select * from zamowieniawidok order by priorytet desc"))
		{
			beginResetModel();	// rozpoczynamy edycję modelu
			m_orders.clear();

			// przeszukujemy pobrane rekordy z bazy
			// i zapisujemy do listy głównej modelu
			while (query.next())
			{
				OrderRecord record;
				record.setTitle(query.value(0).toString());			// opis/tytuł
				record.setCompanyName(query.value(1).toString());	// nazwa firmy
				record.setName(query.value(2).toString());			// imię
				record.setSurname(query.value(3).toString());		// nazwisko
				record.setAddDate(query.value(4).toDate());			// data przyjęcia
				record.setEndDate(query.value(5).toDate());			// termin realizacji
				record.setId(query.value(6).toInt());				// id zamówienia
				record.setDone(query.value(7).toBool());			// czy zamówienie jest zrealizowane
				record.setRabat(query.value(8).toInt());			// wczytuje rabat
				record.setPriotytet(query.value(9).toInt());
				record.setChecked(false);							// przy ładowaniu odznaczamy wszystko

				m_orders.append(record);
			}
			endResetModel();

			return true;
		}
	}

	return false;
}

//--------------------------------------------------
// dodaje zamówienie do bazy oraz zwraca jego ID
int OrdersModel::addOrderToDatabase(const int orderId,
	const int contrahentId, 
	const QDate addDate, 
	const QDate endDate, 
	const QString title, 
	const float saveLength, 
	const bool done,
	const int rabat,
	const QString notes,
	const int priorytet,
	const QString vatText,
	const double price,
	const QList<SquaredRecord> listOfItems,
	const QList<ArticleRecord> articleItems)
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen() && db.isValid())
	{
		QSqlQuery query;
		query.prepare("select dodajZamowienie(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		query.bindValue(0, orderId);
		query.bindValue(1, contrahentId);
		query.bindValue(2, addDate);
		query.bindValue(3, endDate);
		query.bindValue(4, title);
		query.bindValue(5, done);
		query.bindValue(6, saveLength);
		query.bindValue(7, rabat);
		query.bindValue(8, notes);
		query.bindValue(9, priorytet);

		QString str = vatText;
		query.bindValue(10, str.remove(" %").toInt());
		query.bindValue(11, price);

		if (query.exec())
		{
			// zamówienie dodane, dodajemy listę elementów
			int id = 0;
			if (query.first())
				id = query.value(0).toInt();

			// dodajemy elementy do zamówienia
			for (int i = 0; i < listOfItems.size(); i++)
			{
				query.clear();
				query.prepare("call DodajElementZamowienia(?, ?, ?, ?, ?, ?)");
				query.bindValue(0, id);
				query.bindValue(1, listOfItems[i].width());
				query.bindValue(2, listOfItems[i].height());
				query.bindValue(3, listOfItems[i].length());
				query.bindValue(4, listOfItems[i].count());
				query.bindValue(5, listOfItems[i].isPlaned());

				if (!query.exec())
				{
					QMessageBox::critical(0, tr("Błąd"), query.lastError().text());
					break;
				}
			}

			// dodajemy artykuyły do zamówienia
			for (int i = 0; i < articleItems.size(); i++)
			{
				QSqlQuery q;
				q.prepare("INSERT INTO zamowienie_artykuly (idZamowienia, idArtykulu, ilosc) VALUES(?, ?, ?)");
				q.bindValue(0, id);
				q.bindValue(1, articleItems[i].id());
				q.bindValue(2, articleItems[i].count());

				if (!q.exec())
					return 0;
			}

			loadFromDatabase();	// odświerzamy listę po dodaniu
			return id;
		}
		else
		{
			QMessageBox::critical(0, tr("Błąd"), query.lastError().text());
		}
	}
	return 0;
}

//--------------------------------------------------
// ustawia flagę zrealizowania dla konkretnego
// zamówienia
bool OrdersModel::realizeOrder(int id)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery query;
		query.prepare("call Zamowienie_Zrealizuj(?)");
		query.bindValue(0, id);

		if (!query.exec())
		{
			QMessageBox::critical(0, tr("Błąd"), query.lastError().text());
			return false;
		}
	}
	return true;
}

//--------------------------------------------------
// realizuje zaznaczone zamówienia
void OrdersModel::realizeCheckedOrders()
{
	// sprawdzamy czy jest co realizować
	bool bSuccess = false;
	for(int i = 0; i < m_orders.size(); i++)
		if (m_orders[i].isChecked())
		{
			bSuccess = true;
			break;
		}

	// użytkownik wybrał cokolwiek
	if (bSuccess)
	{
		int orderCount = 0;
		for (int i = 0; i < m_orders.size(); i++)
		{
			if (m_orders[i].isChecked())
			{
				if (!realizeOrder(m_orders[i].id()))
					break;

				orderCount++;
			}
		}

		// wyświetlamy informację o zrealizowanych zamówieniach
		QMessageBox::information(0, tr("Informacja"),
			tr("Zrealizowano %1 zamówień").arg(orderCount));

		loadFromDatabase();
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Musisz zaznaczyć przynajmniej jedną pozycję"));

		return;
	}
}

//--------------------------------------------------
// zaznacza wszystkie zamówienia
void OrdersModel::selectAllOrders()
{
	beginResetModel();
	for (int i = 0; i < m_orders.size(); i++)
		m_orders[i].setChecked(true);

	endResetModel();
}

//--------------------------------------------------
// odznacza wszystkie zamówienia na liście
void OrdersModel::unselectAllOrders()
{
	beginResetModel();
	for (int i = 0; i < m_orders.size(); i++)
		m_orders[i].setChecked(false);

	endResetModel();
}

//--------------------------------------------------
// usuwa zaznaczone zamówienia
// pyta użytkownika przed usunięciem
void OrdersModel::removeCheckedOrders()
{
	// sprawdzamy czy użytkownik zanzaczył przynajmniej jedno zamówienie
	bool bSuccess = false;
	for (int i = 0; i < m_orders.size(); i++)
		if (m_orders[i].isChecked())
		{
			bSuccess = true;
			break;
		}

	if (bSuccess)
	{
		// przynajmniej jedno zamówienie zaznaczone
		int iRet = QMessageBox::question(0, tr("Zapytanie"),
			tr("Czy napewno chcesz usunąć zaznaczone zamówienia?\nOperacja nie może być cofnięta !!!"),
			QMessageBox::Yes | QMessageBox::No);

		if (iRet == QMessageBox::Yes)
		{
			QSqlDatabase db = QSqlDatabase::database();
			if (db.isOpen() && db.isValid())
			{
				for (int i = 0; i < m_orders.size(); i++)
				{
					if (m_orders[i].isChecked())
					{
						// zamówienie zaznaczone więc usuwamy
						QSqlQuery q;
						q.prepare("call usunZamowienie(?)");
						q.bindValue(0, m_orders[i].id());

						if (!q.exec())
						{
							QMessageBox::critical(0, tr("Błąd"), tr("Błąd podczas próby usunięcia zamówienia!!!\n"
								"Operacja została przerwana."));

							break;
						}
					}
				}
				loadFromDatabase();	// wczytujemy dane ponownie
			}
			else
			{
				QMessageBox::critical(0, tr("Błąd"),
					tr("Urtacono połączenie z bazą danych"));

				return;
			}
		}
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Zanim cokolwiek usuniesz musisz wybrać które zamówienia chcesz usunąć"));

		return;
	}
}
