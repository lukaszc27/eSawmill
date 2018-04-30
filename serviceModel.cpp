#include "serviceModel.h"
#include <qmessagebox.h>
#include <qbrush.h>
#include <qcolor.h>


ServiceModel::ServiceModel(QObject* parent)
	: QAbstractTableModel(parent)
{
	m_headerList << tr("Tytuł") << tr("Firma") << tr("Imię") << tr("Nazwisko") 
		<< tr("Data dodania") << tr("Termin") << tr("Priorytet");

	loadFromDatabase();
}

//---------------------------------------------------------
// ustawia odpowiednie flagi dla odpowiednich komurek
Qt::ItemFlags ServiceModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);
	
	f |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (index.column() == 0)
		f |= Qt::ItemIsUserCheckable;

	return f;
}

//---------------------------------------------------------
// wraca ilość wierszy w modelu
int ServiceModel::rowCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_items.size();
}

//---------------------------------------------------------
// zwraca liczbę kolumn w modelu
int ServiceModel::columnCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_headerList.size();
}

//---------------------------------------------------------
// umieszcza odpowiednie dane w odpowiednich komurkach w modelu
QVariant ServiceModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case 0: return m_items[index.row()].title(); break;
		case 1: return m_items[index.row()].company(); break;
		case 2: return m_items[index.row()].name(); break;
		case 3: return m_items[index.row()].surname(); break;
		case 4: return m_items[index.row()].addDate(); break;
		case 5: return m_items[index.row()].endDate(); break;
		case 6:
			switch (m_items[index.row()].priorytet())
			{
			case 0: return tr("NISKI"); break;
			case 1: return tr("NORMALNY"); break;
			case 2: return tr("WYSOKI"); break;
			}
			break;
		}
		break;

	case Qt::UserRole:
		return m_items[index.row()].id();
		break;

	case Qt::CheckStateRole:
		if (index.column() == 0)
			return m_items[index.row()].isChecked() ? Qt::Checked : Qt::Unchecked;
		break;

	case Qt::TextColorRole:
		if (m_items[index.row()].isDone())
			return QVariant::fromValue(QBrush(QColor(128, 128, 128)));
		break;

	case Qt::BackgroundRole:
		// podświetlamy na czerwono jeśli minoł termin realizacji
		if (m_items[index.row()].endDate() <= QDate::currentDate() && !m_items[index.row()].isDone())
			return QVariant::fromValue(QBrush(Qt::red));
		break;
	}
	return QVariant();
}

//---------------------------------------------------------
// wyświetla tytuły kolumn
QVariant ServiceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_headerList.at(section);

	return QVariant();
}

//---------------------------------------------------------
// wczytuje listę zapisaną w bazie danych
bool ServiceModel::loadFromDatabase()
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen() && db.isValid())
	{
		m_items.clear();	 // czyścimy listę przed ponownym uzupełnieniem

		QSqlQuery q;
		if (q.exec("select * from uslugiwidok"))
		{
			beginResetModel();
			while (q.next())
			{
				ServiceRecord r;
				r.setTitle(q.value(0).toString());
				r.setCompany(q.value(1).toString());
				r.setName(q.value(2).toString());
				r.setSurname(q.value(3).toString());
				r.setAddDate(q.value(4).toDate());
				r.setEndDate(q.value(5).toDate());
				r.setDone(q.value(7).toBool());
				r.setId(q.value(8).toInt());
				r.setPriorytet(q.value(11).toInt());
				r.setChecked(false);
				m_items.append(r);
			}
			endResetModel();

			return true;
		}
		else
		{
			QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
			return false;
		}
	}
	return false;
}

//---------------------------------------------------------
// zapisuje wartości do modelu
bool ServiceModel::setData(const QModelIndex & index, const QVariant & value, int role)
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

//---------------------------------------------------------
// dodaje nową usługe lub jeżeli istnieje ona w bazie
// to uaktualnia
void ServiceModel::addService(const QString title, 
	const QDate addDate, 
	const QDate endDate, 
	const bool done, 
	const double rabat,
	const QString notes,
	const QList<RoundRecord> rounds,
	const QList<SquaredRecord> squards,
	const QList<ArticleRecord> articles,
	const int contrahentId, 
	const int id,
	const int priorytet,
	const int vat)
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen() && db.isValid())
	{
		int serviceId = 0;
		QSqlQuery q;
		q.prepare("select DodajUsluge(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		q.bindValue(0, id);
		q.bindValue(1, contrahentId);
		q.bindValue(2, title);
		q.bindValue(3, addDate);
		q.bindValue(4, endDate);
		q.bindValue(5, done);
		q.bindValue(6, rabat);
		q.bindValue(7, priorytet);
		q.bindValue(8, vat);
		q.bindValue(9, notes);

		if (!q.exec())
		{
			QMessageBox::critical(0, tr("Błąd"),
				q.lastError().text());

			return;
		}
		else
		{
			if (q.first())
				serviceId = q.value(0).toInt();

			// czyścimy tabele z kłodami jeśli wcześniej się tam coś znajdowało
			q.clear();
			q.prepare("call wyczyscUsluge(?)");
			q.bindValue(0, serviceId);

			if (!q.exec())
			{
				QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
				return;
			}

			// dodajemy kłody do bazy
			for (int i = 0; i < rounds.size(); i++)
			{
				q.clear();
				q.prepare("insert into klodyuslugi (idUslugi, srednica, dlugosc, kantowana) "
					"values(?, ?, ?, ?)");
				q.bindValue(0, serviceId);
				q.bindValue(1, rounds[i].diameter());
				q.bindValue(2, rounds[i].length());
				q.bindValue(3, rounds[i].isEaves());

				if (!q.exec())
				{
					QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
					break;
				}
			}

			// zapisujemy elementy w bazie
			for (int i = 0; i < squards.size(); i++)
			{
				q.clear();
				q.prepare("insert into elementyuslugi (idUslugi, szerokosc, wysokosc, dlugosc, ilosc, strugane) "
					"values(?, ?, ?, ?, ?, ?)");
				q.bindValue(0, serviceId);
				q.bindValue(1, squards[i].width());
				q.bindValue(2, squards[i].height());
				q.bindValue(3, squards[i].length());
				q.bindValue(4, squards[i].count());
				q.bindValue(5, squards[i].isPlaned());

				if (!q.exec())
				{
					QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
					return;
				}
			}

			// zapisujemy artykuły w bazie
			for (int i = 0; i < articles.size(); i++)
			{
				q.clear();
				q.prepare("INSERT INTO uslugi_artykuly (idUslugi, idArtykulu, ilosc) VALUES(?, ?, ?)");
				q.bindValue(0, serviceId);
				q.bindValue(1, articles[i].id());
				q.bindValue(2, articles[i].count());

				if (!q.exec())
				{
					QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
					return;
				}
			}

			loadFromDatabase();	// wczytujemy dane z bazy ponownie
		}
	}
}

//---------------------------------------------------------
// oznacza jako realizowane wszystkie zaznaczone usługi
void ServiceModel::realiseCheckedItems()
{
	// sprawdzamy czy zaznaczono przynajmniej jedną
	bool bSuccess = false;
	for(int i = 0; i < m_items.size(); i++)
		if (m_items[i].isChecked())
		{
			bSuccess = true;
			break;
		}
	
	if (bSuccess)
	{
		// nawiązujemy połączenie z bazą danych
		QSqlDatabase db = QSqlDatabase::database();
		if (db.isOpen() && db.isValid())
		{
			// szukamy zaznaczonych usług
			for (int i = 0; i < m_items.size(); i++)
				if (m_items[i].isChecked())
				{
					QSqlQuery q;
					q.prepare("update uslugi set zrealizowane = 1 where id = ?");
					q.bindValue(0, m_items[i].id());

					if (!q.exec())
					{
						QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
						break;
					}
				}
		}
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Przed wykonaniem tej operacji musisz zaznaczyć przynajmniej jedną usługe"));
		return;
	}
}

//---------------------------------------------------------
// zaznacza wszystkie dostępne usługi
void ServiceModel::selectAll()
{
	beginResetModel();
	for (int i = 0; i < m_items.size(); i++)
		m_items[i].setChecked(true);
	endResetModel();
}

//---------------------------------------------------------
// odznacza wszystkie dostępne usługi
void ServiceModel::unselectAll()
{
	beginResetModel();
	for (int i = 0; i < m_items.size(); i++)
		m_items[i].setChecked(false);
	endResetModel();
}

//---------------------------------------------------------
// usuwa usługi z bazy oraz odświeża główna listę
void ServiceModel::removeCheckedItems()
{
	// sprawdzamy czy cokolwiek zostało zaznaczone na liście
	bool bSuccess = false;
	for (int i = 0; i < m_items.size(); i++)
		if (m_items[i].isChecked())
		{
			bSuccess = true;
			break;
		}

	if (bSuccess)
	{
		QSqlDatabase db = QSqlDatabase::database();
		if (db.isValid() && db.isOpen())
		{
			int iRet = QMessageBox::question(0, tr("Pytanie"),
				tr("Czy napewno chcesz usunąć zaznaczone elemeny?\n"
					"Operacja nie może być cofnięta!!!"),
				QMessageBox::Yes | QMessageBox::No);

			if (iRet == QMessageBox::Yes)
			{
				for (int i = 0; i < m_items.size(); i++)
				{
					if (m_items[i].isChecked())
					{
						QSqlQuery q;
						q.prepare("call usunUsluge(?)");
						q.bindValue(0, m_items[i].id());

						if (!q.exec())
							break;
					}
				}
				loadFromDatabase();	// odświerzamy główna listę
			}
		}
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Przed usunięciem musisz wybrać przynajmniej jedną pozycje do usunnięcia!"));

		return;
	}
}
