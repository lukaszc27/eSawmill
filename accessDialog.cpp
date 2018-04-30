#include "accessDialog.h"
#include <qpushbutton.h>
#include <qsettings.h>

AccessDialog::AccessDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(tr("eSawmill - Logowanie do serwera MySQL"));
	setMinimumWidth(300);

	createWidgets();

	// wczytanie konfiguracji
	QSettings settings;
	if (settings.value("access/rememberMe").toBool())
	{
		m_hostName->setText(settings.value("access/host").toString());
		m_userName->setText(settings.value("access/userName").toString());
		m_password->setText(settings.value("access/password").toString());
		m_rememberMe->setChecked(settings.value("access/rememberMe").toBool());
	}

	connect(m_buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

//----------------------------------------------
// zwraca bezpiecznie podaną nazwę użytkownika
QString AccessDialog::userName() const
{
	return m_userName->text();
}

//----------------------------------------------
// zwraca bezpiecznie podane hasło
QString AccessDialog::password() const
{
	return m_password->text();
}

//----------------------------------------------
// zwraca bezpiecznie host
QString AccessDialog::hostName() const
{
	return m_hostName->text();
}

//----------------------------------------------
// zwraca bezpiecznie czy użytkownik ma zostać zapamiętany
bool AccessDialog::rememberMe() const
{
	return m_rememberMe->isChecked();
}

//----------------------------------------------
// tworzenie widgetów w oknie
void AccessDialog::createWidgets()
{
	m_hostName		= new QLineEdit(this);
	m_userName		= new QLineEdit(this);
	m_password		= new QLineEdit(this);
	m_rememberMe	= new QCheckBox(tr("Pamiętaj mnie"), this);
	m_buttons		= new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

	// ustawiamy ikony dla przycisków
	m_buttons->button(QDialogButtonBox::Ok)->setIcon(QIcon("icons/ok.png"));
	m_buttons->button(QDialogButtonBox::Cancel)->setIcon(QIcon("icons/cancel.png"));

	m_password->setEchoMode(QLineEdit::Password);	// ukrywamy znaki w polu hasło

	// ustawienie elementów przez layout
	QFormLayout* form = new QFormLayout;
	form->addRow(tr("Host"), m_hostName);
	form->addRow(tr("Użytkownik"), m_userName);
	form->addRow(tr("Hasło"), m_password);

	QVBoxLayout* main = new QVBoxLayout(this);
	main->addLayout(form);
	main->addWidget(m_rememberMe);
	main->addWidget(m_buttons);
}
