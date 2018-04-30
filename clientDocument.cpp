#include "clientDocument.h"
#include <qtextstream.h>
#include <qsettings.h>
#include <qapplication.h>


ClientDocument::ClientDocument(QObject* parent)
	: QTextDocument(parent)
{
	m_documentDate = QDate::currentDate();	// pobieramy aktualną dane
	setTextWidth(10);						// wysokość tekstu
	setDefaultFont(QFont("Courier New"));	// czciąka dla całego dokumentu

	// wczytujemy ustawienia z konfiguracji
	QSettings settings;
	m_companyName = settings.value("app/companyName").toString();
	m_ownName = settings.value("app/ownName").toString();
	m_ownSurname = settings.value("app/ownSurname").toString();
	m_nip = settings.value("app/nip").toString();
	m_regon = settings.value("app/regon").toString();
	m_pesel = settings.value("app/pesel").toString();
	m_city = settings.value("app/city").toString();
	m_zipCode = settings.value("app/zipCode").toString();
	m_zipName = settings.value("app/zipName").toString();
	m_street = settings.value("app/street").toString();
	m_houseNumber = settings.value("app/houseNumber").toString();
	m_phoneNumber = settings.value("app/phoneNumber").toString();
	m_mail = settings.value("app/mail").toString();
	m_website = settings.value("app/website").toString();
}

//---------------------------------------------
// zwraca blok prezentujący dane firmowe
QString ClientDocument::companyBlock()
{
	QString block;
	QTextStream out(&block);

	// ustalamy wielkość czciąki
	out << "<p style=\"font-size: 10px;\">";

	// nazwa firmy jeśli podano
	if (!m_companyName.isEmpty())
		out << "<strong>" << m_companyName.toUpper() << "</strong><br/>";

	// imię i nazwisko właściciela jeśli podano
	if (!m_ownName.isEmpty() && !m_ownSurname.isEmpty())
		out << m_ownName.toUpper() << " " << m_ownSurname.toUpper() << "<br/>";

	// nazwa miejscowości
	if (!m_city.isEmpty())
		out << m_city.toUpper() << " ";

	// ulica
	if (!m_street.isEmpty())
		out << "ul. " << m_street.toUpper() << " ";

	// numer lokalu/domu
	if (!m_houseNumber.isEmpty())
		out << m_houseNumber << "<br/>";

	// kod poczotwy
	if (!m_zipCode.isEmpty())
		out << m_zipCode << " ";

	// nazwa poczty
	if (!m_zipName.isEmpty())
		out << m_zipName.toUpper() << "<br/>";

	// NIP
	if (!m_nip.isEmpty())
		out << "NIP: " << m_nip << "<br/>";

	// REGON
	if (!m_regon.isEmpty())
		out << "REGON: " << m_regon << "<br/>";

	// PESEL
	if (!m_pesel.isEmpty())
		out << "PESEL: " << m_pesel << "<br/>";

	// mail
	if (!m_mail.isEmpty())
		out << "E-Mail: " << m_mail.toUpper() << "<br/>";

	// strona internetowa
	if (!m_website.isEmpty())
		out << "www: " << m_website.toUpper() << "<br/>";

	// telefon
	if (!m_phoneNumber.isEmpty())
		out << "Tel.: " << m_phoneNumber.toUpper() << "<br/>";

	out << "</p>";

	return block;	// zwrawcamy utowrzony blok danych
}

//---------------------------------------------
// przygotowuje dokument do druku
// ustawia prawidłowy wygląd faktury
void ClientDocument::prepareToPrint(const QString contrahentBlock, const QString itemsBlock, const QString other)
{
	QSettings settings;
	QString document;
	QTextStream out(&document);

	// główne rozłożenie elementów na podstawie tabeli
	out << "<table width=\"100%\">";
	out << "<tr><td>";
	
	// wyświetlenie logo firmy
	out << "<img src=\"" << settings.value("app/banner").toString() << "\" />";
	// informacje o firmie
	out << companyBlock();
	
	out << "</td><td>";
	
	// miejsce oraz data zawarcia/podpisania umowy(rozliczenia)
	out << "<p align=\"right\" style=\"font-size: 10px;\">" << m_city.toUpper() << " dn. " <<
		m_documentDate.day() << "." << m_documentDate.month() << "." << m_documentDate.year() << "</p>";
	
	out << "</td></tr>";
	out << "<tr><td colspan=\"2\">";
	
	// wyświetlenie danych odbiercy (kontrahenta)
	if (!contrahentBlock.isEmpty())
	{
		out << "<center><table width=\"70%\">";
		out << "<tr><td width=\"65%\"></td><td><b>ODBIORCA</b></td></tr>";
		out << "<tr><td></td><td>" << contrahentBlock << "</td></tr>";
		out << "</table></center>";
	}

	out << "</td></tr>";
	out << "<tr><td colspan=\"2\">";
	
	// tytuł dokumentu
	out << "<br><h2><p align=\"center\">" << m_documentTitle.toUpper() << "</p></h2>";
	
	out << "</td></tr>";
	out << "<tr><td colspan=\"2\">";
	
	// wstawiamy listę elementów razem z podliczeniem kosztów
	out << "<br><center>" << itemsBlock << "</center>";

	out << "</td></tr>";
	out << "<tr><td colspan=\"2\">";
	
	// tu wyś=wietlamy dodatkowe informacje które chce wydrukować użytkownik
	out << other;

	out << "</td></tr>";
	out << "<tr><td>";
	out << "<p style=\"font-size: 9px; color: #808080;\">Wydrukowano z programu " << QApplication::applicationName()
		<< " wer. " << QApplication::applicationVersion() << "</p>";
	out << "</td></tr>";
	out << "</table>";

	// na końcu wstawiamy przygotowany szablon do dokumentu głównego
	setHtml(document);
}
