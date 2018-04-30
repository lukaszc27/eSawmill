#include "contrahentModel.h"
#include <qmessagebox.h>


ContrahentModel::ContrahentModel(QObject* parent, bool selectedMode)
	: QAbstractTableModel(parent)
	, m_selectedMode(selectedMode)
{
	m_db = QSqlDatabase::database();

	// nadanie nazw kolumną
	m_headerLabels << tr("Firma") << tr("Imię") << tr("Nazwisko") << tr("NIP") << tr("REGON") << tr("PESEL")
		<< tr("Kod pocztowy") << tr("Poczta") << tr("Miejscowość") << tr("Ulica") << tr("Numer lokalu/domu")
		<< tr("Telefon") << tr("E-Mail") << tr("Strona WWW");
}

//----------------------------------------------
// liczba wierszy w modelu
int ContrahentModel::rowCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_contrahents.size();
}

//----------------------------------------------
// liczba kolumn w modelu
int ContrahentModel::columnCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_headerLabels.size();
}

//----------------------------------------------
// właściwości dla pojedynczej komurki elementu
Qt::ItemFlags ContrahentModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);
	f |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (!m_selectedMode)
		f |= Qt::ItemIsEditable;

	if (index.column() == 0)
		f |= Qt::ItemIsUserCheckable;

	return f;
}

//----------------------------------------------
// wyświetlanie danych w modelu
QVariant ContrahentModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case 0: return m_contrahents.at(index.row()).companyName(); break;	// nazwa firmy
		case 1: return m_contrahents.at(index.row()).name(); break;			// imię
		case 2: return m_contrahents.at(index.row()).surname(); break;		// nazwisko
		case 3: return m_contrahents.at(index.row()).nip(); break;			// NIP
		case 4: return m_contrahents.at(index.row()).regon(); break;		// REGON
		case 5: return m_contrahents.at(index.row()).pesel(); break;		// PESEL
		case 6: return m_contrahents.at(index.row()).zipCode(); break;		// kod pocztowy
		case 7: return m_contrahents.at(index.row()).zipName(); break;		// nazwa poczty
		case 8: return m_contrahents.at(index.row()).city(); break;			// miejscowość
		case 9: return m_contrahents.at(index.row()).street(); break;		// ulica
		case 10: return m_contrahents.at(index.row()).houseNumber(); break;	// numer domu/mieszkania
		case 11: return m_contrahents.at(index.row()).phone(); break;		// telefon
		case 12: return m_contrahents.at(index.row()).mail(); break;		// adres e-mail
		case 13: return m_contrahents.at(index.row()).website(); break;;	// strona WWW
		}
		break;

	case Qt::UserRole:
		return m_contrahents.at(index.row()).id();	// zwracamy id konkretnego kontrahenta
		break;

	case Qt::CheckStateRole:	// możliwość zaznaczenia kontrahentów
		if (index.column() == 0)
			return m_contrahents.at(index.row()).isChecked() ? Qt::Checked : Qt::Unchecked;
		break;

	case Qt::TextColorRole:
		if (m_contrahents[index.row()].isDebator())
			return QVariant::fromValue(QBrush(Qt::blue));
		break;
	}

	return QVariant();
}

//----------------------------------------------
// wyświetlanie paska tytułowego dla nazw kolumn
QVariant ContrahentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_headerLabels.at(section);

	return QVariant();
}

//----------------------------------------------
// zapewnia możliwość edycji danych w modelu
bool ContrahentModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	switch (role)
	{
	case Qt::CheckStateRole:
		if (index.column() == 0)
			m_contrahents[index.row()].setChecked(value.toBool());
		break;

	case Qt::EditRole:
		if (m_db.isOpen() && m_db.isValid())
		{
			QSqlQuery q;
			switch (index.column())
			{
			case 0:	// nazwa firmy
				q.clear();
				q.prepare("update kontrahent set firma=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setCompanyName(value.toString());
				break;

			case 1:	// imię
				q.clear();
				q.prepare("update kontrahent set imie=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setName(value.toString());
				break;

			case 2:	// nazwisko
				q.clear();
				q.prepare("update kontrahent set nazwisko=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setSurname(value.toString());
				break;

			case 3:	// nip
				q.clear();
				q.prepare("update kontrahent set nip=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setNip(value.toString());
				break;

			case 4:	// regon
				q.clear();
				q.prepare("update kontrahent set regon=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setRegon(value.toString());
				break;

			case 5:	// pesel
				q.clear();
				q.prepare("update kontrahent set pesel=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setPesel(value.toString());
				break;

			case 6:	// kod pocztowy
				q.clear();
				q.prepare("update adres set kod_pocztowy=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setZipCode(value.toString());
				break;

			case 7:	// poczta
				q.clear();
				q.prepare("update adres set poczta=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setZipName(value.toString());
				break;

			case 8:	// miejscowość
				q.clear();
				q.prepare("update adres set miejscowosc=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setCity(value.toString());
				break;

			case 9:	// ulica
				q.clear();
				q.prepare("update adres set ulica=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());
				
				if (q.exec())
					m_contrahents[index.row()].setStreet(value.toString());
				break;

			case 10:	// numer lokalu
				q.clear();
				q.prepare("update adres set nr_lokalu=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setHouseNumber(value.toString());
				break;

			case 11:	// telefon
				q.clear();
				q.prepare("update adres set telefon=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setPhone(value.toString());
				break;

			case 12:	// e-mail
				q.clear();
				q.prepare("update adres set email=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setMail(value.toString());
				break;

			case 13:	// strona www
				q.clear();
				q.prepare("update adres set www=? where kontrahentId=?");
				q.bindValue(0, value.toString());
				q.bindValue(1, m_contrahents[index.row()].id());

				if (q.exec())
					m_contrahents[index.row()].setWebsite(value.toString());
				break;
			}
		}
		break;
	}
	return true;
}

//----------------------------------------------
// zwraca listę z tytułami poszczegulnych kolumn
QStringList ContrahentModel::headerLabels() const
{
	return m_headerLabels;
}

//----------------------------------------------
// oznacza osobe o podanym Id jako dłużnika
bool ContrahentModel::markDebator(int id)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery query;
		query.prepare("call Kontrahent_OznaczDluznika(?)");
		query.bindValue(0, id);

		if (!query.exec())
			return false;
	}
	return true;
}

//----------------------------------------------
// zdejmuje niechlubną nazę dłużnika
bool ContrahentModel::setNormalContrahent(int id)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery query;
		query.prepare("update kontrahent set dluznik = 0 where kontrahentId = ?");
		query.bindValue(0, id);

		if (!query.exec())
			return false;
	}
	return true;
}

//----------------------------------------------
// wczytuje nowe dane do modelu
// usuwając wcześniejsze jeżeli się znajdują
bool ContrahentModel::loadFromDatabase()
{
	if (m_db.isOpen() && m_db.isValid())
	{
		// czyścimy śmieci jeśli istnieją
		if (m_contrahents.size() > 0)
			m_contrahents.clear();

		// wczytujemy nowe dane do modelu
		QSqlQuery query;
		if (query.exec("select * from kontrahenci"))
		{
			beginResetModel();
			while (query.next())
			{
				ContrahentRecord record;
				record.setId(query.value(0).toInt());
				record.setCompanyName(query.value(1).toString());
				record.setName(query.value(2).toString());
				record.setSurname(query.value(3).toString());
				record.setNip(query.value(4).toString());
				record.setRegon(query.value(5).toString());
				record.setPesel(query.value(6).toString());
				record.setZipCode(query.value(7).toString());
				record.setZipName(query.value(8).toString());
				record.setCity(query.value(9).toString());
				record.setStreet(query.value(10).toString());
				record.setHouseNumber(query.value(11).toString());
				record.setPhone(query.value(12).toString());
				record.setMail(query.value(13).toString());
				record.setWebsite(query.value(14).toString());
				record.setDebator(query.value(15).toBool());
				record.setChecked(false);

				m_contrahents.append(record);
			}
			endResetModel();
			return true;	// powodzenie wczytania danych
		}
		else
		{
			QMessageBox::critical(0, tr("SQL Syntax Error!!!"), query.lastError().text());
			return false;
		}
	}
	else
	{
		QMessageBox::critical(0, tr("Database Server Error!!!"), m_db.lastError().text());
		return false;
	}
	return false;
}

//----------------------------------------------
// dodaje nowego kontrahenta do bazy MySQL
// i wczytuje wszystkich na nowo
bool ContrahentModel::addNewContrahent(const QString companyName, 
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
	const QString website)
{
	if (m_db.isValid() && m_db.isOpen())
	{
		QSqlQuery query;
		query.prepare("select DodajKontrahenta(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		query.bindValue(0, companyName);
		query.bindValue(1, name);
		query.bindValue(2, surname);
		query.bindValue(3, nip);
		query.bindValue(4, regon);
		query.bindValue(5, pesel);
		query.bindValue(6, zipCode);
		query.bindValue(7, zipName);
		query.bindValue(8, city);
		query.bindValue(9, street);
		query.bindValue(10, houseNumber);
		query.bindValue(11, phone);
		query.bindValue(12, mail);
		query.bindValue(13, website);

		if (!query.exec())
		{
			QMessageBox::critical(0, tr("SQL Syntax Error!!!"), query.lastError().text());
			return false;
		}
		else loadFromDatabase();	// jeśli wszystko się powiodło wczytujemy na nowo dane z bazy w celu aktualizacji
	}
	return true;
}

//----------------------------------------------
// usuwa zaznaczonych kontrahentów z bazy
// oraz odświerza listę
void ContrahentModel::removeContrahent()
{
	int result = QMessageBox::question(0, tr("Usuwanie kontrahentów - Pytanie"),
		tr("Czy napewno chcesz usunąć zaznaczonych kontrachentów?\nOperacja jest nie odwracalna!!!"),
		QMessageBox::Yes | QMessageBox::No);

	if (result == QMessageBox::Yes)
	{
		if (m_db.isOpen() && m_db.isValid())
		{
			// sprawdzamy czy użytkownik zaznaczył przynajmniej jednego kontrahenta
			bool isSelected = false;
			for (int i = 0; i < m_contrahents.size(); i++)
				if (m_contrahents.at(i).isChecked())
				{
					isSelected = true;
					break;
				}

			if (isSelected)
			{
				// użytkownik dokonał wyboru
				bool bSuccess = true;	// czy wszyscy zostali prawidłowo usunięci
				for (int i = 0; i < m_contrahents.size(); i++)
				{
					if (m_contrahents.at(i).isChecked())
					{
						QSqlQuery query;
						query.prepare("call usunKontrahenta(?)");
						query.bindValue(0, m_contrahents.at(i).id());

						if (!query.exec())
						{
							QMessageBox::critical(0, tr("SQL Syntax Error!!!"), query.lastError().text());
							bSuccess = false;
							return;
						}
					}
				}
				if (bSuccess)
					loadFromDatabase();	// odświerzamy listę po usunięciu wszystkich zaznaczonych kontrachentów
			}
			else
			{
				QMessageBox::information(0, tr("Information"),
					tr("Przed usunięciem musisz zaznaczyć przynajmniej jednego kontrahenta"));
			}
		}
	}
}

//----------------------------------------------
// oznacza zaznaczone osoby jako dłużników
void ContrahentModel::markDebatorCheckedContrahents()
{
	// sprawdzamy czy ktokolwiek jest zaznaczony
	bool bSuccess = false;
	for (int i = 0; i < m_contrahents.size(); i++)
		if (m_contrahents[i].isChecked())
		{
			bSuccess = true;
			break;
		}

	// zaznaczono przynajmniej jedną osobę
	if (bSuccess)
	{
		for (int i = 0; i < m_contrahents.size(); i++)
			if (m_contrahents[i].isChecked())
				if (!markDebator(m_contrahents[i].id()))
					break;

		loadFromDatabase();
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Musisz zaznaczyć przynajmniej jedną osobe"));

		return;
	}
}

//----------------------------------------------
// usuwa flagę dłużnika dla zaznaczonych osób
void ContrahentModel::removeCheckedDebators()
{
	// sprawdzenie czy cokolwiek jest zaznaczone
	bool bSuccess = false;
	for(int i = 0; i < m_contrahents.size(); i++)
		if (m_contrahents[i].isChecked())
		{
			bSuccess = true;
			break;
		}

	if (bSuccess)
	{
		for (int i = 0; i < m_contrahents.size(); i++)
			if (m_contrahents[i].isChecked())
				if (!setNormalContrahent(m_contrahents[i].id()))
					break;

		loadFromDatabase();
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Musisz zaznaczyć przynajmniej jedną osobe"));

		return;
	}
}

//----------------------------------------------
// zaznacza wszystkie rekordy
void ContrahentModel::selectAll()
{
	beginResetModel();
	for (int i = 0; i < m_contrahents.size(); i++)
		m_contrahents[i].setChecked(true);

	endResetModel();
}

//----------------------------------------------
// odznacza wszystkie rekordy
void ContrahentModel::unselectAll()
{
	beginResetModel();
	for (int i = 0; i < m_contrahents.size(); i++)
		m_contrahents[i].setChecked(false);

	endResetModel();
}
