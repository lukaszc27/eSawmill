#include "configurationDialog.h"
#include <qlayout.h>
#include <qmessagebox.h>


ConfigurationDialog::ConfigurationDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Konfiguracja"));

	// tworzenie stron konfiguracji
	m_mainPage		= new ConfigurationMainPage(this);
	m_orderPage		= new ConfigurationOrderPage(this);
	m_servicePage	= new ConfigurationServicePage(this);
	m_mailPage		= new ConfigurationMailPage(this);

	createWidgets();

	connect(m_buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

//-----------------------------------------
// zapisanie konfiguracji
void ConfigurationDialog::accept()
{
	// zapis danych z wszystkich oknien konfiguracyjnych
	QList<ConfigurationWidget*> configWidgets = findChildren<ConfigurationWidget*>();
	for(int i = 0; i < configWidgets.size(); i++)
		configWidgets.at(i)->save();

	QDialog::accept();
}

//-----------------------------------------
// tworzy layout głównego okna konfiguracji
void ConfigurationDialog::createWidgets()
{
	m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	m_tabWidget = new QTabWidget(this);

	// wyszukanie oraz dodanie okien konfiguracyjnych
	QList<ConfigurationWidget*> configWidgets = findChildren<ConfigurationWidget*>();
	for(int i = 0; i < configWidgets.size(); i++) {
		m_tabWidget->addTab(configWidgets.at(i),
							configWidgets.at(i)->windowIcon(),
							configWidgets.at(i)->windowTitle());
	}

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(m_tabWidget);
	mainLayout->addWidget(m_buttons);
}
