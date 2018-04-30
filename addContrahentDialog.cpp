#include "addContrahentDialog.h"
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qvalidator.h>


AddContrahentDialog::AddContrahentDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Dodawanie nowego kontrahenta"));
	createWidgets();

	connect(m_buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_buttons, SIGNAL(rejected()), this, SLOT(close()));
}

//-------------------------------------------------
// torzenie widgetów używanych w dialogu
void AddContrahentDialog::createWidgets()
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

	createVialidators();	// tworzymy walidatory danych

	// layout układający pola wprowadzania danych
	QGridLayout* form = new QGridLayout;
	form->addWidget(new QLabel(tr("Imię"), this),		0, 0);	form->addWidget(m_name, 0, 1);
	form->addWidget(new QLabel(tr("Nazwisko"), this),	1, 0);	form->addWidget(m_surname, 1, 1);
	form->addWidget(new QLabel(tr("Firma"), this),		2, 0);	form->addWidget(m_companyName, 2, 1);
	form->addWidget(new QLabel(tr("NIP"), this),		3, 0);	form->addWidget(m_nip, 3, 1);
	form->addWidget(new QLabel(tr("REGON"), this),		4, 0);	form->addWidget(m_regon, 4, 1);
	form->addWidget(new QLabel(tr("PESEL"), this),		5, 0);	form->addWidget(m_pesel, 5, 1);
	form->addWidget(new QLabel(tr("Miejscowość"), this),6, 0);	form->addWidget(m_city, 6, 1);

	form->addWidget(new QLabel(tr("Kod poczotwy"), this),	0, 2);	form->addWidget(m_zipCode, 0, 3);
	form->addWidget(new QLabel(tr("Poczta"), this),			1, 2);	form->addWidget(m_zipName, 1, 3);
	form->addWidget(new QLabel(tr("Ulica"), this),			2, 2);	form->addWidget(m_street, 2, 3);
	form->addWidget(new QLabel(tr("Numer domu"), this),		3, 2);	form->addWidget(m_houseNumber, 3, 3);
	form->addWidget(new QLabel(tr("Telefon"), this),		4, 2);	form->addWidget(m_phone, 4, 3);
	form->addWidget(new QLabel(tr("E-Mail"), this),			5, 2);	form->addWidget(m_mail, 5, 3);
	form->addWidget(new QLabel(tr("Strona WWW"), this),		6, 2);	form->addWidget(m_website, 6, 3);

	m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

	m_buttons->button(QDialogButtonBox::Ok)->setIcon(QIcon("icons/ok.png"));
	m_buttons->button(QDialogButtonBox::Cancel)->setIcon(QIcon("icons/cancel.png"));

	// główny layout układający widgety w oknie
	QVBoxLayout* main = new QVBoxLayout(this);
	main->addLayout(form);
	main->addWidget(m_buttons);
}

//-------------------------------------------------
// tworzy walidatory danych do elementów
// wprowadzania danych
void AddContrahentDialog::createVialidators()
{
	// walidator dla danych tekstowych (pozwala wprowadzić tylko tekst)
	QRegExpValidator* onlyTextValidator = new QRegExpValidator(QRegExp("[A-Za-z-]*"), this);

	// przypisanie walidaora do elementów
	m_name->setValidator(onlyTextValidator);
	m_surname->setValidator(onlyTextValidator);
	m_city->setValidator(onlyTextValidator);
	m_zipName->setValidator(onlyTextValidator);

	// walidator dla pola "Nazwa firmy"
	QRegExpValidator* onlyCompanyNameValidator = new QRegExpValidator(QRegExp("[A-Za-z0-9\\s]*"), this);
	m_companyName->setValidator(onlyCompanyNameValidator);

	// walidator dla pola PESEL
	m_pesel->setValidator(new QRegExpValidator(QRegExp("[0-9]{11}"), this));

	// walidator dla pola NIP
	m_nip->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,3}-[0-9]{1,3}-[0-9]{1,3}-[0-9]{1,3}"), this));
	m_nip->setPlaceholderText(tr("000-000-000-000"));
	// walidaotr dla pola REGON
	m_regon->setValidator(new QRegExpValidator(QRegExp("[0-9]{9}"), this));

	// walidator dla kodu pocztowego
	m_zipCode->setValidator(new QRegExpValidator(QRegExp("[0-9]{2}-[0-9]{3}"), this));
	m_zipCode->setPlaceholderText(tr("00-000"));

	// mail walidator
	m_mail->setValidator(new QRegExpValidator(QRegExp("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}\\b"), this));

	// telefon walidator
	m_phone->setValidator(new QRegExpValidator(QRegExp("[0-9]{3}[\\s]?[0-9]{3}[\\s]?[0-9]{3}")));

	// numer domu/mieszkania
	m_houseNumber->setValidator(new QRegExpValidator(QRegExp("[0-9]*[A-Za-z]?"), this));

	// walidator adresu WWW
	m_website->setValidator(new QRegExpValidator(QRegExp("www\\.[A-Za-z0-9-]*\\.[A-Za-z0-9\\./]*"), this));
}

//-------------------------------------------------
// bezpieczne pobanie imienia
QString AddContrahentDialog::name() const
{
	return m_name->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nazwiska
QString AddContrahentDialog::surname() const
{
	return m_surname->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nazwy firmy
QString AddContrahentDialog::companyName() const
{
	return m_companyName->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nip-u
QString AddContrahentDialog::nip() const
{
	return m_nip->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie numeru REGON
QString AddContrahentDialog::regon() const
{
	return m_regon->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie numery PESEL
QString AddContrahentDialog::pesel() const
{
	return m_pesel->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie kodu pocztowego
QString AddContrahentDialog::zipCode() const
{
	return m_zipCode->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie bazyw poczty
QString AddContrahentDialog::zipName() const
{
	return m_zipName->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nazwy miejscowości
QString AddContrahentDialog::city() const
{
	return m_city->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie nazwy ulicy
QString AddContrahentDialog::street() const
{
	return m_street->text().toUpper();
}

//-------------------------------------------------
// bezpieczne pobranie numeru domu/mieszkania
QString AddContrahentDialog::houseNumber() const
{
	return m_houseNumber->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie numeru telefonu
QString AddContrahentDialog::phone() const
{
	return m_phone->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie adresu e-mail
QString AddContrahentDialog::mail() const
{
	return m_mail->text().toUpper();
}

//-------------------------------------------------
// bezpieczne zwrucenie adresu strony WWW
QString AddContrahentDialog::website() const
{
	return m_website->text().toUpper();
}
