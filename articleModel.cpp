#include "articleModel.h"
#include <qmessagebox.h>
#include <qtextstream.h>


ArticleModel::ArticleModel(QObject* parent)
	: QAbstractTableModel(parent)
	, m_isReadOnly(false)
{
	m_headerList << tr("Nazwa") << tr("Jm") << tr("Cena netto") << tr("Cena brutto") << tr("VAT")
		<< tr("Marża") << tr("Cena detaliczna") << tr("Ilość") << tr("Wartość");

	m_db = QSqlDatabase::database();
}

//---------------------------------------------------------
// zwraca liczbę wierszy w modelu
int ArticleModel::rowCount(const QModelIndex & index) const
{
	Q_UNUSED(index);

	return m_items.size();
}

//---------------------------------------------------------
// zwraca iczbę kolumn w modelu
int ArticleModel::columnCount(const QModelIndex & index) const
{
	Q_UNUSED(index);

	return m_headerList.size();
}

//---------------------------------------------------------
// flagi edycji dla poszczególnych komórek tabeli
Qt::ItemFlags ArticleModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f = QAbstractTableModel::flags(index);

	if (!m_isReadOnly)
	{
		f |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		if (index.column() == 0)
			f |= Qt::ItemIsUserCheckable;
		else if (index.column() == 7)
			f |= Qt::ItemIsEditable;

	}
	else f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	return f;
}

//---------------------------------------------------------
// umieszcza dane w odpowiednich kolumnach
QVariant ArticleModel::data(const QModelIndex & index, int role) const
{
	int col = index.column();
	int row = index.row();
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
		case 7: return m_items[row].count(); break;
		case 8:
			return m_items[row].detalPrice()*m_items[row].count();
			break;
		}
		break;

	case Qt::CheckStateRole:
		if (col == 0)
			return m_items[row].isChecked() ? Qt::Checked : Qt::Unchecked;
		break;
	}
	return QVariant();
}

//---------------------------------------------------------
// mieszcza odpowiednie tytuły w kolumnach
QVariant ArticleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return QString(m_headerList.at(section));

	return QVariant();
}

//---------------------------------------------------------
// umożliwia edycję danych w modelu
// oraz zaznaczenie/odznaczenie pozycji
bool ArticleModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	switch (role)
	{
	case Qt::CheckStateRole:
		if (index.column() == 0)
			m_items[index.row()].setChecked(value.toBool());
		break;

	case Qt::EditRole:
		if (index.column() == 7)
			m_items[index.row()].setCount(value.toDouble());
		break;
	}
	emit modelChanged();
	return true;
}

//---------------------------------------------------------
// dodaje nowy rekord do modelu
void ArticleModel::addItem(const QString & name, const QString & jm, const double netto, 
	const double brutto, const double vat, const int magazineId, const float detalPrice, 
	const float marza, const float count, const int articleId)
{
	beginResetModel();
	
	ArticleRecord r;
	r.setName(name);
	r.setJm(jm);
	r.setNettoPrice(netto);
	r.setBruttoPrice(brutto);
	r.setVat(vat);
	r.setMagazineId(magazineId);
	r.setDetalPrie(detalPrice);
	r.setMarza(marza);
	r.setCount(count);
	r.setId(articleId);

	m_items.append(r);	// dodajemy element do listy
	emit modelChanged();

	endResetModel();
}

//---------------------------------------------------------
// wczytuje elementy z bazy
bool ArticleModel::loadItemsFromOrder(int orderId)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery q;
		q.prepare("SELECT zamowienie_artykuly.ilosc, artykuly.nazwa, artykuly.cenaNetto, artykuly.cenaBrutto, "
			"artykuly.vat, artykuly.jm, artykuly.cenaDetaliczna, artykuly.marza, artykuly.idMagazynu, artykuly.idArtykulu "
			"FROM artykuly, zamowienie_artykuly "
			"WHERE artykuly.idArtykulu = zamowienie_artykuly.idArtykulu AND zamowienie_artykuly.idZamowienia = ?");
		
		q.bindValue(0, orderId);

		if (q.exec())
		{
			while (q.next())
				addItem(q.value(1).toString(), q.value(5).toString(), q.value(2).toDouble(), 
					q.value(3).toDouble(), q.value(4).toInt(), q.value(8).toInt(), q.value(6).toDouble(), 
					q.value(7).toDouble(), q.value(0).toDouble(), q.value(9).toInt());

			return true;

		}
		else
		{
			QMessageBox::critical(0, tr("Błąd"),
				q.lastError().text());

			return false;
		}
	}
}

//---------------------------------------------------------
// odczytuje zapisane elementy z uslugi
bool ArticleModel::loadItemsFromService(int serviceId)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery q;
		q.prepare("SELECT uslugi_artykuly.ilosc, artykuly.nazwa, artykuly.cenaNetto, artykuly.cenaBrutto, "
			"artykuly.vat, artykuly.jm, artykuly.cenaDetaliczna, artykuly.marza, artykuly.idMagazynu, artykuly.idArtykulu "
			"FROM artykuly, uslugi_artykuly "
			"WHERE artykuly.idArtykulu = uslugi_artykuly.idArtykulu AND uslugi_artykuly.idUslugi = ?");

		q.bindValue(0, serviceId);

		if (q.exec())
		{
			while(q.next())
				addItem(q.value(1).toString(), q.value(5).toString(), q.value(2).toDouble(),
					q.value(3).toDouble(), q.value(4).toInt(), q.value(8).toInt(), q.value(6).toDouble(),
					q.value(7).toDouble(), q.value(0).toDouble(), q.value(9).toInt());

			return true;
		}
		else
		{
			QMessageBox::critical(0, tr("Błąd"),
				q.lastError().text());

			return false;
		}
	}
}

//---------------------------------------------------------
// zwraca całkowitą cene do zapłaty
double ArticleModel::priceToPay()
{
	double price = 0;

	for (int i = 0; i < m_items.size(); i++)
	{
		double x = m_items[i].detalPrice() * m_items[i].count();
		price += x;
	}
	return price;
}

//---------------------------------------------------------
// przygotowuję listę do druku
QString ArticleModel::itemsToHtml()
{
	QString html;
	QTextStream out(&html);

	out << "<table border=\"1\" style=\"font-size: 10px;\" width=\"80%\">";
	out << "<tr><th>" << tr("Lp") << "</th>"
		<< "<th width=\"50%\">" << tr("Nazwa") << "</th>"
		<< "<th>" << tr("Ilość") << "</th>"
		<< "<th>" << tr("Jm") << "</th>"
		<< "<th>" << tr("Cena") << "</th>"
		<< "<th>" << tr("Wartość") << "</th></tr>";

	for (int i = 0; i < m_items.size(); i++)
	{
		out << "<tr>";

		out << "<td>" << i + 1 << "</td>";
		out << "<td>" << m_items[i].name() << "</td>";
		out << "<td>" << m_items[i].count() << "</td>";
		out << "<td>" << m_items[i].jm() << "</td>";
		out << "<td>" << m_items[i].detalPrice() << "</td>";
		out << "<td>" << m_items[i].detalPrice()*m_items[i].count() << "</td>";
			
		out << "</tr>";
	}
	out << "<tr><td colspan=\"5\"><p style=\"text-align: right;\"><b>" << tr("DO ZAPŁATY") << "</b></td>";
	out << "<td><b>" << priceToPay() << "</b></td></tr>";

	out << "</table>";

	return html;
}

//---------------------------------------------------------
// usuwa zaznaczone elementy
void ArticleModel::removeCheckedItem()
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
		beginResetModel();
		QList<ArticleRecord> tmp = m_items;	// lista tymczasowa
		m_items.clear();					// czyścimy całą listę główna

		for (int i = 0; i < tmp.size(); i++)
		{
			if (!tmp[i].isChecked())
				m_items.append(tmp[i]);
		}
		emit modelChanged();
		endResetModel();
	}
	else
	{
		QMessageBox::warning(0, tr("Ostrzeżenie"),
			tr("Zanim coś usuniesz musisz najpierw wybrać elementy do usunięcia"));

		return;
	}
}
