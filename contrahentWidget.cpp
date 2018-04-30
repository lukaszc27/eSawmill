#include "contrahentWidget.h"
#include <qgridlayout.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include "contrahentManager.h"

ContrahentWidget::ContrahentWidget(QWidget* parent)
	: QWidget(parent)
{
	m_db = QSqlDatabase::database();
	createWidgets();

	connect(m_chooseButton, SIGNAL(clicked()), this, SLOT(openContrahentManager()));
}

//-------------------------------------------------
// tworzenie widgetów wprowadzania/wyświetlania danych
void ContrahentWidget::createWidgets()
{
	m_name			= new QLineEdit(this);
	m_surname		= new QLineEdit(this);
	m_companyName	= new QLineEdit(this);
	m_nip			= new QLineEdit(this);
	m_regon			= new QLineEdit(this);
	m_pesel			= new QLineEdit(this);
	m_zipCode		= new QLineEdit(this);
	m_zipName		= new QLineEdit(this);
	m_city			= new QLineEdit(this);
	m_street		= new QLineEdit(this);
	m_houseNumber	= new QLineEdit(this);
	m_phone			= new QLineEdit(this);
	m_mail			= new QLineEdit(this);
	m_website		= new QLineEdit(this);
	m_chooseButton	= new QPushButton(QIcon("icons/contrahents.png"), tr("..."), this);

	// blokujemy możVliwość wprowadzenia danych
	QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();
	for (int i = 0; i < lineEdits.size(); i++)
		lineEdits.at(i)->setReadOnly(true);

	QHBoxLayout* nameLayout = new QHBoxLayout;
	nameLayout->addWidget(m_name);
	nameLayout->addWidget(m_chooseButton);

	// layout układający pola wprowadzania danych
	QGridLayout* form = new QGridLayout(this);
	form->addWidget(new QLabel(tr("Imię"), this), 0, 0);		form->addLayout(nameLayout, 0, 1);
	form->addWidget(new QLabel(tr("Nazwisko"), this), 1, 0);	form->addWidget(m_surname, 1, 1);
	form->addWidget(new QLabel(tr("Firma"), this), 2, 0);		form->addWidget(m_companyName, 2, 1);
	form->addWidget(new QLabel(tr("NIP"), this), 3, 0);			form->addWidget(m_nip, 3, 1);
	form->addWidget(new QLabel(tr("REGON"), this), 4, 0);		form->addWidget(m_regon, 4, 1);
	form->addWidget(new QLabel(tr("PESEL"), this), 5, 0);		form->addWidget(m_pesel, 5, 1);
	form->addWidget(new QLabel(tr("Miejscowość"), this), 6, 0);	form->addWidget(m_city, 6, 1);

	form->addWidget(new QLabel(tr("Kod poczotwy"), this), 0, 2);	form->addWidget(m_zipCode, 0, 3);
	form->addWidget(new QLabel(tr("Poczta"), this), 1, 2);			form->addWidget(m_zipName, 1, 3);
	form->addWidget(new QLabel(tr("Ulica"), this), 2, 2);			form->addWidget(m_street, 2, 3);
	form->addWidget(new QLabel(tr("Numer domu"), this), 3, 2);		form->addWidget(m_houseNumber, 3, 3);
	form->addWidget(new QLabel(tr("Telefon"), this), 4, 2);			form->addWidget(m_phone, 4, 3);
	form->addWidget(new QLabel(tr("E-Mail"), this), 5, 2);			form->addWidget(m_mail, 5, 3);
	form->addWidget(new QLabel(tr("Strona WWW"), this), 6, 2);		form->addWidget(m_website, 6, 3);
}

//-------------------------------------------------
// otwiera menadżera kontrahentów w trybie
// wyboru oraz łączy odpowiednie sygnały z akcjami
void ContrahentWidget::openContrahentManager()
{
	ContrahentManager* manager = new ContrahentManager(this, true);
	connect(manager, SIGNAL(contrahentSelected(int)), this, SLOT(loadContrahent(int)));

	manager->exec();	// wyświetlamy menadżera jako okno modalne
}

//-------------------------------------------------
// wczytuje z bazy MySQL kontrahenta o podanym ID
void ContrahentWidget::loadContrahent(int id)
{
	if (m_db.isOpen() && m_db.isValid())
	{
		m_contrahentId = id;	// przypisanie id kontrahenta

		QSqlQuery q;
		q.prepare("select * from kontrahenci where id=?");
		q.bindValue(0, id);

		if (q.exec())
		{
			if (q.first())
			{
				m_companyName->setText(q.value(1).toString());
				m_name->setText(q.value(2).toString());
				m_surname->setText(q.value(3).toString());
				m_nip->setText(q.value(4).toString());
				m_regon->setText(q.value(5).toString());
				m_pesel->setText(q.value(6).toString());
				m_zipCode->setText(q.value(7).toString());
				m_zipName->setText(q.value(8).toString());
				m_city->setText(q.value(9).toString());
				m_street->setText(q.value(10).toString());
				m_houseNumber->setText(q.value(11).toString());
				m_phone->setText(q.value(12).toString());
				m_mail->setText(q.value(13).toString());
				m_website->setText(q.value(14).toString());
			}
		}
		else
		{
			QMessageBox::critical(this, tr("SQL Syntax Error!!!"), q.lastError().text());
			return;
		}
	}
}

//-------------------------------------------------
// bezpieczne pobanie imienia
QString ContrahentWidget::name() const
{
	return m_name->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nazwiska
QString ContrahentWidget::surname() const
{
	return m_surname->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nazwy firmy
QString ContrahentWidget::companyName() const
{
	return m_companyName->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nip-u
QString ContrahentWidget::nip() const
{
	return m_nip->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie numeru REGON
QString ContrahentWidget::regon() const
{
	return m_regon->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie numery PESEL
QString ContrahentWidget::pesel() const
{
	return m_pesel->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie kodu pocztowego
QString ContrahentWidget::zipCode() const
{
	return m_zipCode->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie bazyw poczty
QString ContrahentWidget::zipName() const
{
	return m_zipName->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nazwy miejscowości
QString ContrahentWidget::city() const
{
	return m_city->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nazwy ulicy
QString ContrahentWidget::street() const
{
	return m_street->text().toUpper();
}

//-------------------------------------------------
// bezpieczne pobranie numeru domu/mieszkania
QString ContrahentWidget::houseNumber() const
{
	return m_houseNumber->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie numeru telefonu
QString ContrahentWidget::phone() const
{
	return m_phone->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie adresu e-mail
QString ContrahentWidget::mail() const
{
	return m_mail->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie adresu strony WWW
QString ContrahentWidget::website() const
{
	return m_website->text().toUpper();
}

//-------------------------------------------------
// tworzy cały adres kontrahenta
// w widoku do nadruku na kopertach
QString ContrahentWidget::contrahentAddress() const
{
	QString address;
	QTextStream out(&address);

	// ustalamy wielkość czciąki
	out << "<p style=\"font-size: 10px;\">";

	// nazwa firmy jeśli podano
	if (!companyName().isEmpty())
		out << "<strong>" << companyName() << "</strong><br/>";

	// imię i nazwisko właściciela jeśli podano
	if (!name().isEmpty() && !surname().isEmpty())
		out << name() << " " << surname() << "<br/>";

	// nazwa miejscowości
	if (!city().isEmpty())
		out << city() << " ";

	// ulica
	if (!street().isEmpty())
		out << "ul. " << street() << " ";

	// numer lokalu/domu
	if (!houseNumber().isEmpty())
		out << houseNumber() << "<br/>";

	// kod poczotwy
	if (!zipCode().isEmpty())
		out << zipCode() << " ";

	// nazwa poczty
	if (!zipName().isEmpty())
		out << zipName() << "<br/>";

	// NIP
	if (!nip().isEmpty())
		out << "NIP: " << nip() << "<br/>";

	// REGON
	if (!regon().isEmpty())
		out << "REGON: " << regon() << "<br/>";

	// PESEL
	if (!pesel().isEmpty())
		out << "PESEL: " << pesel() << "<br/>";

	// mail
	if (!mail().isEmpty())
		out << "E-Mail: " << mail() << "<br/>";

	// strona internetowa
	if (!website().isEmpty())
		out << "www: " << website() << "<br/>";

	// telefon
	if (!phone().isEmpty())
		out << "Tel.: " << phone() << "<br/>";

	out << "</p>";

	return address;
}
