#include "squaredModel.h"
#include <qmessagebox.h>
#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qsettings.h>


SquaredModel::SquaredModel(QObject* parent)
	: QAbstractTableModel(parent)
	, m_vat(0)
	, m_vatEnable(false)
{
	m_db = QSqlDatabase::database();	// pobieramy uchwyt na aktualne połączenie z MySQL

	QSettings settings;
	m_price = settings.value("squaredPrice").toDouble();

	m_readOnly = false;
	m_headerLabels << tr("Szerokość [cm]") << tr("Wysokość [cm]") << tr("Długość [m]") << tr("Ilość [szt]")
		<< tr("Strugane") << tr("Kubatura [m3]");
}
//---------------------------------------------------------
// zwraca liczbę wierszy w tabeli
int SquaredModel::rowCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_items.size();
}

//---------------------------------------------------------
// zwraca liczbę kolumn w tabeli
int SquaredModel::columnCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_headerLabels.size();
}

//---------------------------------------------------------
// ustawia odpowiednie flagi dla poszczególnych komórek
Qt::ItemFlags SquaredModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);
	if (!m_readOnly)
	{
		if (index.column() < 5)
			f |= Qt::ItemIsEnabled | Qt::ItemIsEditable;
		else f |= Qt::NoItemFlags;

		if (index.column() == 0)
			f |= Qt::ItemIsUserCheckable;
	}
	else f |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	return f;
}

//---------------------------------------------------------
// umieszcza dane pobrane z listy w odpowiednich komórkach
QVariant SquaredModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case 0:	// szerokość
			return m_items[index.row()].width();
			break;

		case 1:	// wysokość
			return m_items[index.row()].height();
			break;

		case 2:	// długość
			return m_items[index.row()].length();
			break;

		case 3:	// ilość
			return m_items[index.row()].count();
			break;

		case 4:	// strugane
			return m_items[index.row()].isPlaned() ? tr("TAK") : tr("NIE");
			break;

		case 5:	// kubatura
			float width = m_items[index.row()].width() / 100;
			float height = m_items[index.row()].height() / 100;
			float length = m_items[index.row()].length();
			float count = m_items[index.row()].count();

			float m = (width*height*length)*count;	// kubatura elementu

			return QVariant::fromValue(m);
			break;
		}
		break;

	case Qt::CheckStateRole:	// dodajemy możliwość zaznaczenia wiersza
		if (index.column() == 0)
			return m_items[index.row()].isChecked() ? Qt::Checked : Qt::Unchecked;
		break;

	case Qt::EditRole:	// zwracamy informacje czy dany element ma być strugany czy nie
		if (index.column() == 4)
			return m_items[index.row()].isPlaned();
		break;

	case Qt::BackgroundRole:	// jeśli element ma być strugany podświetlamy go lekko na szaro
		if (m_items[index.row()].isPlaned())
			return QVariant::fromValue(QBrush(QColor(236, 236, 236)));
		break;
	}
	return QVariant();
}

//---------------------------------------------------------
//  umieszcza tytuły dla poszczególnych kolumn
QVariant SquaredModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_headerLabels[section];

	return QVariant();
}

//---------------------------------------------------------
// nadaje nowe wartości elementą w liscie m_items
// pomocne przy programowaniu MVC
bool SquaredModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	switch (role)
	{
	case Qt::CheckStateRole:
		if (index.column() == 0)
			m_items[index.row()].setChecked(value.toBool());
		break;

	case Qt::EditRole:
		switch (index.column())
		{
		case 0: m_items[index.row()].setWidth(value.toDouble()); break;
		case 1: m_items[index.row()].setHeight(value.toDouble()); break;
		case 2: m_items[index.row()].setLength(value.toDouble()); break;
		case 3: m_items[index.row()].setCount(value.toInt()); break;
		case 4: m_items[index.row()].setPlaned(value.toBool()); break;
		}
		break;

	default:
		QAbstractTableModel::setData(index, value, role);
	}
	emit modelChanged();	// inforujemy o zmianie danych w modelu

	return true;
}

//---------------------------------------------------------
// dodaje nowy element do listy
void SquaredModel::addItem(float width, float height, float length, float count, bool planed, bool checked)
{
	beginResetModel();
	
	SquaredRecord record(width, height, length, count, planed, checked);	// tworzymy tymczasowy rekord
	m_items.append(record);	// a następnie dodajemy go do listy głównej
	emit modelChanged();

	endResetModel();
}

//---------------------------------------------------------
// dodaje nowy element do listy
void SquaredModel::addItem(SquaredRecord record)
{
	addItem(record.width(), record.height(), record.length(), record.count(), record.isPlaned());
}

//---------------------------------------------------------
// wylicza oraz zwraca całkowitą ilość m3 w zamówieniu
double SquaredModel::sumOfMetric()
{
	double sum = 0.0;

	for(int i = 0; i < m_items.size(); i++)
		sum += m_items[i].metric();

	return sum;	// zwracamy liczbę m3
}

//---------------------------------------------------------
// ilość m3 drewna struganego
double SquaredModel::planedOfMetric()
{
	double sum = 0.0;

	for(int i = 0; i < m_items.size(); i++)
		if(m_items[i].isPlaned())
			sum += m_items[i].metric();

	return sum;
}

//---------------------------------------------------------
// wyliczamy cenę za elementy konstrukcyjne
// bez uwzględnienia rabatu
double SquaredModel::price()
{
	QSettings settings;
	double planedPrice = settings.value("planedPrice").toDouble();

	float price = (sumOfMetric() * m_price) + (planedOfMetric() * planedPrice);
	
/*	if (m_rabat < 0)
		m_rabat = 0;

	float rabat = 100 - m_rabat;
	float x = rabat / 100;
	float toPay = price * x;
*/

	return price;
}

//---------------------------------------------------------
// zapisuje wprowadzone dane w formacie XML
bool SquaredModel::exportToXML(const QString& fileName)
{
	QDomDocument document;
	QDomElement root = document.createElement("squaredItems");

	// dodajemy główny element dokumentu
	document.appendChild(root);

	// oraz pozostałe elementy
	for (int i = 0; i < m_items.size(); i++)
	{
		QDomElement node = document.createElement("item");
		node.setAttribute("width", m_items[i].width());
		node.setAttribute("height", m_items[i].height());
		node.setAttribute("length", m_items[i].length());
		node.setAttribute("count", m_items[i].count());
		node.setAttribute("planed", m_items[i].isPlaned());

		root.appendChild(node);	// dodajemy do głównego elementu
	}

	// zapisujemy dokument XML do pliku
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream fileOut(&file);
		fileOut << document.toString();

		file.close();
		return true;
	}
	else
	{
		QMessageBox::critical(0, tr("Błąd"),
			tr("Podczas tworzenia pliku doszło do nieoczekiwanego błędu :("));

		file.close();
		return false;
	}
	return false;
}

//---------------------------------------------------------
// importuje dane z pliku XML oraz umieszcza w głównej liscie
bool SquaredModel::importFromXML(const QString & fileName)
{
	if (!m_readOnly)
	{
		if (m_items.size() > 0)
		{
			int iRet = QMessageBox::question(0, tr("Zapytanie"), tr("Dopisać importowane elementy do aktualnych?"),
				QMessageBox::Yes | QMessageBox::No);

			if (iRet == QMessageBox::No)	// jeśli mamy nie dopisywać to czyścimy listę
				m_items.clear();
		}

		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QDomDocument document;
			if (document.setContent(&file))
			{
				QDomElement root = document.firstChildElement();		// pobieramy główny element
				QDomNodeList items = root.elementsByTagName("item");	// pobieramy listę elementów

				for (int i = 0; i < items.count(); i++)
				{
					if (items.at(i).isElement())
					{
						// konwertujemy do elementu
						QDomElement element = items.at(i).toElement();

						// dodajemy element do listy głównej
						addItem(element.attribute("width").toDouble(),
							element.attribute("height").toDouble(),
							element.attribute("length").toDouble(),
							element.attribute("count").toDouble(),
							element.attribute("planed").toInt());
					}
				}
				file.close();
				return true;
			}
			else
			{
				QMessageBox::critical(0, tr("Błąd"), tr("Wystąpił błąd podczas parsowania pliku XML"));
				file.close();

				return false;
			}
		}
		else
		{
			QMessageBox::critical(0, tr("Błąd"),
				tr("Podczas otwierania pliku wystąpił jakiś nieoczekiwany błąd :("));

			return false;
		}
	}
	else
	{
		QMessageBox::warning(0, tr("ostrzeżenie"),
			tr("Nie możesz zaimportować danych w trybie do odczytu"));

		return false;
	}
	return false;
}

//---------------------------------------------------------
// wczytuje listę elementów z bazy po id zamówienia
bool SquaredModel::loadFromOrder(const int id)
{
	// sprawdzenie czy połączenie z bazą nawiązane
	if (m_db.isOpen() && m_db.isValid())
	{
		// połączenie nawiązane, wczytujemy dane
		QSqlQuery query;
		query.prepare("select szerokosc, wysokosc, dlugosc, ilosc, strugane "
			"from elementyzamowienia where zamowienieId = ?");

		query.bindValue(0, id);
		if (query.exec())
		{
			// wczytano elementy zapisujemy do listy głównej
			while (query.next())
			{
				addItem(query.value(0).toDouble(), query.value(1).toDouble(), query.value(2).toDouble(),
					query.value(3).toDouble(), query.value(4).toDouble());
			}
		}
		emit modelChanged();	 // emitujemy sygnał informujący ze w modelu zaszły zmiany
		return true;
	}

	return false;
}

//---------------------------------------------------------
// wczytuje dane z usługi
bool SquaredModel::loadFromService(const int id)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery q;
		q.prepare("select szerokosc, wysokosc, dlugosc, ilosc, strugane "
			"from elementyuslugi where idUslugi = ?");
		q.bindValue(0, id);

		if (q.exec())
		{
			// dane pobrane z bazy
			// wczytujemy do listy
			m_items.clear();
			while (q.next())
			{
				addItem(q.value(0).toDouble(), q.value(1).toDouble(), q.value(2).toDouble(),
					q.value(3).toDouble(), q.value(4).toDouble());
			}
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------
// zapisuje listę elementów razem z podliczeniem kosztów
// wykorzystywana przy drukowaniu dokumentów
QString SquaredModel::itemsToHtml()
{
	QString document;
	QTextStream out(&document);
	QSettings settings;

	out << "<p style=\"font-size: 10px;\">";
	out << "<table border=\"1\">";
	out << "<tr><th>Lp</th>"
		<< "<th>Wymiar<br>(szer x wys)<br>[cm x cm]</th>"
		<< tr("<th>Długość<br>[m]</th>")
		<< tr("<th>Ilość<br>[szt]</th>")
		<< "<th>Kubatura<br>(1 szt)<br>[m<sup>3</sup>]</th>"
		<< tr("<th>Kubatura<br>(szt x ilość)<br>[m<sup>3</sup>]</th>");

	if (m_vatEnable)
	{
		out << tr("<th>Wartość netto<br>[zł]</th>")
			<< tr("<th>Wartość VAT<br>[%]</th>")
			<< tr("<th>Wartość brutto<br>[zł]</th></tr>");
	}
	else out << tr("<th>Wartość<br>[zł]</th></tr>");

	// przeszukujemy całą listę elementów
	// i wpisujemy do tabeli
	for (int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i].isPlaned())	// jeśli element jest strugany to podświetlamy go na szaro
			out << "<tr style=\"background-color: #e6e6e6;\">";
		else out << "<tr>";

		out << "<td>" << i + 1 << "</td>";	// liczba porządkowa
		out << "<td>" << m_items[i].width() << "x" << m_items[i].height() << "</td>";
		out << "<td>" << m_items[i].length() << "</td>";
		out << "<td>" << m_items[i].count() << "</td>";

		// wyliczamy ilość m3 sztuki
		double x = ((m_items[i].width()/100)*(m_items[i].height()/100)*m_items[i].length());
		double netto = x * m_items[i].count() * m_price;
		double vat = 100 + m_vat;
		double brutto = (netto * vat)/100;

		out << "<td>" << x << "</td>";						// kubatura szt.
		out << "<td>" << x * m_items[i].count() << "</td>";	// kubatura całości
		
		if (m_vatEnable)
		{
			out << "<td>" << netto << "</td>";				// warość netto
			out << "<td>" << m_vat << "%" << "</td>";		// stawka VAT
			out << "<td>" << brutto << "</td>";				// wartość brutto
		}
		else
		{
			out << "<td>" << netto << "</td>";	// jeśli wat nie jest uwzględniany wyświetlamy tylko cene netto
		}

		out << "</tr>";	// cały wiersz wprowadzony
	}

	// podliczenie kosztów
	if (m_vatEnable)
	{
		double netto = sumOfMetric() * m_price;
		double vat = 100 + m_vat;
		double brutto = (netto * vat) / 100;

		out << "<tr><td colspan=\"5\"><p align=\"right\"><b>RAZEM</b></p></td>"
			<< "<td>" << sumOfMetric() << "</td>"
			<< "<td>" << netto << "</td>"
			<< "<td>" << m_vat << "%" << "</td>"
			<< "<td>" << brutto << "</td></tr>";

		netto = planedOfMetric() * settings.value("planedPrice").toDouble();
		brutto = (netto * vat) / 100;

		out << "<tr><td colspan=\"5\"><p align=\"right\"><b>STRUGANIE</b></p></td>"
			<< "<td>" <<planedOfMetric() << "</td>"
			<< "<td>" << netto << "</td>"
			<< "<td>" << m_vat << "%</td>"
			<< "<td>" << brutto << "</td></tr>";

		netto = price();
		brutto = (netto * vat) / 100;

		out << "<tr><td colspan=\"6\"><p align=\"right\"><b>" << tr("DO ZAPŁATY") << "</b></p></td>"
			<< "<td><b>" << netto << "</b></td>"
			<< "<td><b>" << m_vat << "%</b></td>"
			<< "<td><b>" << brutto << "</td></tr>";
	}
	else
	{
		out << "<tr><td colspan=\"5\"><p align=\"right\"><b>RAZEM</b></p></td>"
			<< "<td>" << sumOfMetric() << "</td>"
			<< "<td>" << sumOfMetric() * m_price << "</td></tr>";

		out << "<tr><td colspan=\"5\"><p align=\"right\"><b>STRUGANIE</b></p></td>"
			<< "<td>" << planedOfMetric() << "</td><td>" << planedOfMetric() * settings.value("planedPrice").toDouble() << "</td></tr>";
	
		out << "<tr><td colspan=\"6\"><p align=\"right\"><b>" << tr("DO ZAPŁATY") << "</b></p></td>"
			<< "<td><b>" << price() << "</b></td></tr>";
	}

	out << "</table></p>";

	return document;
}

//---------------------------------------------------------
// zaznacza wszystkie elementy na liście
void SquaredModel::selectAll()
{
	beginResetModel();

	for (int i = 0; i < m_items.size(); i++)
		m_items[i].setChecked(true);

	endResetModel();
}

//---------------------------------------------------------
// odznacza wszystkie elementy na liście
void SquaredModel::unselectAll()
{
	beginResetModel();

	for (int i = 0; i < m_items.size(); i++)
		m_items[i].setChecked(false);

	endResetModel();
}

//---------------------------------------------------------
// usuwa zaznaczone elementy po odczymaniu zgody
void SquaredModel::removeCheckedItems()
{
	// sprawdzamy czy użxytkownik zaznaczył cokolwiek
	bool bSuccess = false;
	for(int i = 0; i < m_items.size(); i++)
		if (m_items[i].isChecked())
		{
			bSuccess = true;
			break;
		}

	if (bSuccess)
	{
		// jeśli zaznaczono przynajmniej jeden element
		// pytamy czy napewno usuwać ?

		int iResult = QMessageBox::question(0, tr("Zapytanie"),
			tr("Czy na pewno chcesz usunąć zaznaczone elementy z listy?\r\n"
				"Operacja nie może być cofnięta!!!"),
			QMessageBox::Yes | QMessageBox::No);

		if (iResult == QMessageBox::Yes)
		{
			beginResetModel();

			// zgoda na usunięcie, więc usuwam
			QList<SquaredRecord> tmp = m_items;
			m_items.clear();	// czyścimy listę główna

			for (int i = 0; i < tmp.size(); ++i)
			{
				if (!tmp[i].isChecked())
					m_items.append(tmp[i]);
			}

			endResetModel();
		}
		emit modelChanged();
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Zanim usuniesz elementy musisz wybrać które."));
		return;
	}
}


