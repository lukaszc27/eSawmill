#include "roundModel.h"
#include "roundRecord.h"
#include <qxml.h>
#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qsettings.h>

const double Pi = 3.14159265358979323846;

RoundModel::RoundModel(QObject* parent)
	: QAbstractTableModel(parent)
	, m_readOnly(false)
	, m_rabat(0)
	, m_vat(0)
	, m_vatEnabled(false)
{
	m_headerLabels << tr("Średnica [cm]") << tr("Długość [m]") << tr("Kantowana") << tr("Kubatura [m3]");
}

//-------------------------------------------------------
// zwraca ilość wierszy w modelu
int RoundModel::rowCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_items.size();
}

//-------------------------------------------------------
// zwraca ilość kolumn w modelu
int RoundModel::columnCount(const QModelIndex & index) const
{
	if (index.isValid())
		return 0;

	return m_headerLabels.size();
}

//-------------------------------------------------------
// flagi dla poszczegulnych elementów w modelu
Qt::ItemFlags RoundModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);
	f |= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

	if (!m_readOnly)
	{
		if (index.column() == 0)
			f |= Qt::ItemIsUserCheckable;
		else if (index.column() == 3)
			f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	else f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	return f;
}

//-------------------------------------------------------
// umieszcza odpowiednie dane w odpowiednich miejscach
QVariant RoundModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case Diameter: return m_items[index.row()].diameter(); break;
		case Length: return m_items[index.row()].length(); break;
		case Eaves: return m_items[index.row()].isEaves() ? "TAK" : "NIE"; break;
		case Kubatura:
			double diameter = m_items[index.row()].diameter() / 100;	// zamieniamy cm na m
			double r = diameter / 2;									// wyliczamy pomień
			double length = m_items[index.row()].length();

			return (Pi*r*r)*length;
			break;
		}
		break;

	case Qt::CheckStateRole:
		if (index.column() == 0)
			return m_items[index.row()].isChecked() ? Qt::Checked : Qt::Unchecked;
		break;

	case Qt::EditRole:
		if(index.column() == 2)
			return m_items[index.row()].isEaves();
		break;
	}
	return QVariant();
}

//-------------------------------------------------------
// umieszcza tytuły dla poszczególnych kolumn
QVariant RoundModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_headerLabels[section];

	return QVariant();
}

//-------------------------------------------------------
// ustawia nowe wartości do zmiennych w modelu
bool RoundModel::setData(const QModelIndex& index, const QVariant& value, int role)
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
		case Diameter: m_items[index.row()].setDiameter(value.toDouble()); break;
		case Length: m_items[index.row()].setLength(value.toDouble()); break;
		case Eaves: m_items[index.row()].setEaves(value.toBool()); break;
		}
		break;
	}
	emit modelChanged();
	return true;
}

//-------------------------------------------------------
// dodaje pojedynczy element do listy
bool RoundModel::addItem(double diameter, double length, bool eaves, bool checked)
{
	beginResetModel();
	
	RoundRecord r(diameter, length, eaves, checked);
	m_items.append(r);
	emit modelChanged();

	endResetModel();

	return true;
}

//-------------------------------------------------------
// zapisuje elementy do pliku XML
// w celu możliwości puźniejszego odczytu
bool RoundModel::exportToXML(const QString & fileName)
{
	QDomDocument document;
	QDomElement root = document.createElement("rounds");

	document.appendChild(root);
	for (int i = 0; i < m_items.size(); i++)
	{
		QDomElement node = document.createElement("item");
		node.setAttribute("diameter", m_items[i].diameter());
		node.setAttribute("length", m_items[i].length());
		node.setAttribute("eaves", m_items[i].isEaves());
		root.appendChild(node);
	}
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out << document.toString();

		file.close();
		return true;
	}

	return false;
}

//-------------------------------------------------------
// odczytuje elementy z pliku
bool RoundModel::importFromXML(const QString fileName)
{
	if (m_readOnly)
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Nie możesz importować danych w trybie tylko do odczytu!"));

		return false;
	}
	else
	{
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			// sprawdzamy czy są jakieś elementy na liście
			if (m_items.size() > 0)
			{
				// jeśli tak to pytamy czy je usunąć czy dopisać imporotwane dane
				int iRet = QMessageBox::question(0, tr("Pytanie"),
					tr("Czy chcesz dopisać importowane elementy do listy ?"),
					QMessageBox::Yes | QMessageBox::No);

				if (iRet == QMessageBox::No)
					m_items.clear();
			}

			// importujemy dane z pliku XML
			QDomDocument document;
			if (document.setContent(&file))
			{
				QDomElement root = document.firstChildElement("rounds");
				QDomNodeList items = root.elementsByTagName("item");
				
				for (int i = 0; i < items.size(); i++)
				{
					if (items.at(i).isElement())
					{
						QDomElement element = items.at(i).toElement();
						addItem(element.attribute("diameter").toDouble(),
							element.attribute("length").toDouble(),
							element.attribute("eaves").toInt());
					}
				}
				file.close();

				return true;
			}
		}
	}
	return false;
}

//-------------------------------------------------------
bool RoundModel::loadFromService(const int id)
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen() && db.isValid())
	{
		QSqlQuery q;
		q.prepare("select srednica, dlugosc, kantowana from klodyuslugi where idUslugi = ?");
		q.bindValue(0, id);

		if (q.exec())
		{
			// dane pobrane wiec wczytujemy do listy
			m_items.clear();
			while (q.next())
				addItem(q.value(0).toDouble(), q.value(1).toDouble(), q.value(2).toBool());

			emit modelChanged();
			return true;
		}

	}
	return false;
}

//-------------------------------------------------------
// wylicza ilość kubików dodanego drewna (nie kantowanego)
double RoundModel::sumOfMetric()
{
	double sum = 0;

	for(int i = 0; i < m_items.size(); i++)
		if(!m_items[i].isEaves())
			sum += m_items[i].metric();

	return sum;
}

//-------------------------------------------------------
// wylicza ilość (w kubikach) drewna które ma być kantowane
double RoundModel::sumOfEavesMetric()
{
	double sum = 0;

	for(int i = 0; i < m_items.size(); i++)
		if(m_items[i].isEaves())
			sum += m_items[i].metric();

	return sum;
}

//-------------------------------------------------------
// wylicza cenę do zapłaty za wykonanie usługi
double RoundModel::priceOfMetric()
{
	QSettings settings;
	double normalWood = sumOfMetric()*settings.value("roundPrice").toDouble();
	double eavesWood = sumOfEavesMetric()*settings.value("roundEavesPrice").toDouble();
/*
	double rabat = (100 - m_rabat) / 100;
	double price = (normalWood + eavesWood)*rabat;
*/
	double price = normalWood + eavesWood;
	return price;
}

//-------------------------------------------------------
// przepisuje listę do formatu HTML
// puźniej zostaje użyta do druku
QString RoundModel::itemsToHtml()
{
	QString html;
	QTextStream out(&html);

	out << "<table border=\"1\" style=\"font-size: 10px;\">";
	
	out << tr("<tr><th>Lp</th>"
		"<th>Średnica<br>[cm]</th>"
		"<th>Długość<br>[m]</th>"
		"<th>Kubatura<br>[m<sup>3</sup>]</th>"
		"<th>Cena<br>[zł/m<sup>3</sup>]</th>");

	if (isVatEnable())
	{
		out << tr("<th>Wartość netto<br>[zł]</th>"
			"<th>VAT<br>[%]</th>"
			"<th>Wartość brutto<br>[zł]</th></tr>");
	}
	else
	{
		out << tr("<th>Wartość<br>[zł]</th></tr>");
	}

	double sumPrice = 0;
	for (int i = 0; i < m_items.size(); i++)
	{
		out << "<tr>";
		out << "<td>" << i + 1 << "</td>";					// Lp
		out << "<td>" << m_items[i].diameter() << "</td>";	// średnica
		out << "<td>" << m_items[i].length() << "</td>";	// długość

		double d = m_items[i].diameter() / 100;		// cm -> m
		double r = d / 2;							// wyliczenie promienia

		double m = (Pi*(r*r))*m_items[i].length();	// kubikowanie kłdy

		double price = 0;
		out << "<td>" << m << "</td>";				// kubatura
		
		QSettings settings;
		if (m_items[i].isEaves())
		{
			// jeśli kołoda jest kantowana
			out << "<td>" << settings.value("roundEavesPrice").toString() << "</td>";
			
			price = m*settings.value("roundEavesPrice").toDouble();
			sumPrice += price;

			if (isVatEnable())
			{
				double netto = price;
				double vat = 100 + this->vat();
				double brutto = (netto*vat) / 100;

				out << "<td>" << netto << "</td>"
					<< "<td>" << this->vat() << " %</td>"
					<< "<td>" << brutto << "</td>";
			}
			else
			{
				out << "<td>" << price << "</td>";
			}
		}
		else
		{
			out << "<td>" << settings.value("roundPrice").toString() << "</td>";
			price = m*settings.value("roundPrice").toDouble();
			sumPrice += price;

			if (isVatEnable())
			{
				double netto = price;
				double dvat = 100 + vat();
				double brutto = (netto*dvat) / 100;

				out << "<td>" << netto << "</td>"
					<< "<td>" << vat() << " %</td>"
					<< "<td>" << brutto << "</td>";
			}
			else
			{
				out << "<td>" << price << "</td>";
			}
		}

		out << "</tr>";
	}

	out << tr("<tr><td colspan=\"5\"><p align=\"right\"><b>Do zapłaty</b></p></td>");
	if (isVatEnable())
	{
		double netto = sumPrice;
		double dvat = 100 + vat();
		double brutto = (netto*dvat) / 100;

		out << "<td><b>" << netto << "</b></td>"
			<< "<td><b>" << vat() << " %</b></td>"
			<< "<td><b>" << brutto << "</b></td></tr>";
	}
	else
	{
		out << "<td><b>" << sumPrice << "</b></td></tr>";
	}

	out << "</table>";

	return html;
}

//-------------------------------------------------------
// zaznacza wszystkie pozycje na liście
void RoundModel::selectAll()
{
	beginResetModel();
	
	for (int i = 0; i < m_items.size(); i++)
		m_items[i].setChecked(true);
	emit modelChanged();
	
	endResetModel();
}

//-------------------------------------------------------
// odznacza wszystkie elementy na liście
void RoundModel::unselectAll()
{
	beginResetModel();
	
	for (int i = 0; i < m_items.size(); i++)
		m_items[i].setChecked(false);
	emit modelChanged();
	
	endResetModel();
}

//-----------------------------------------
// usuwa z listy zaznaczone elementy
void RoundModel::removeCheckedItems()
{
	// sprawdzamy czy użytkownik zaznaczył przynajmniej 1 element
	bool bSuccess = false;
	for (int i = 0; i < m_items.size(); i++)
		if (m_items[i].isChecked())
			bSuccess = true;

	if (bSuccess)
	{
		int iRet = QMessageBox::question(0, tr("Pytanie"),
			tr("Czy napewno chcesz usunąć zaznaczone elementy?\n"
				"Operacja nie może być cofnięta!!!"),
			QMessageBox::Yes | QMessageBox::No);

		if (iRet == QMessageBox::Yes)
		{
			beginResetModel();
			QList<RoundRecord> tmp = m_items;	// robimy kopię elementów
			m_items.clear();	// czyścimy główna listę

			for (int i = 0; i < tmp.size(); i++)
			{
				if (tmp[i].isChecked())
					continue;
				else m_items.append(tmp[i]);
			}
			emit modelChanged();
			endResetModel();
		}
	}
}
