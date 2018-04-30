#include "configurationDialog.h"
#include <qlayout.h>
#include <qmessagebox.h>


ConfigurationDialog::ConfigurationDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Konfiguracja"));
	createWidgets();

	// tworzenie stron konfiguracji
	m_mainPage = new ConfigurationMainPage(this);
	m_orderPage = new ConfigurationOrderPage(this);
	m_servicePage = new ConfigurationServicePage(this);
	m_mailPage = new ConfigurationMailPage(this);

	m_stackedWidget->addWidget(m_mainPage);
	m_stackedWidget->addWidget(m_orderPage);
	m_stackedWidget->addWidget(m_servicePage);
	m_stackedWidget->addWidget(m_mailPage);

	connect(m_listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
		this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*)));

	connect(m_buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

//-----------------------------------------
// zapisanie konfiguracji
void ConfigurationDialog::accept()
{
	m_mainPage->save();
	m_orderPage->save();
	m_servicePage->save();
	m_mailPage->save();

	QDialog::accept();
}

//-----------------------------------------
// tworzy layout głównego okna konfiguracji
void ConfigurationDialog::createWidgets()
{
	m_stackedWidget = new QStackedWidget(this);
	m_listWidget	= new QListWidget(this);
	m_buttons		= new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

	m_listWidget->setViewMode(QListView::IconMode);
	m_listWidget->setMovement(QListView::Static);
	m_listWidget->setIconSize(QSize(84, 84));
	m_listWidget->setMaximumWidth(96);
	m_listWidget->setSpacing(12);

	QHBoxLayout* centerLayout = new QHBoxLayout;
	centerLayout->addWidget(m_listWidget);
	centerLayout->addWidget(m_stackedWidget);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(centerLayout);
	mainLayout->addWidget(m_buttons);

	createWidgetsForMenu();	// tworzymy elementy menu
}

//-----------------------------------------
// tworzy widgety dla menu konfiguracji
void ConfigurationDialog::createWidgetsForMenu()
{
	m_mainMenuItem = new QListWidgetItem(m_listWidget);
	m_mainMenuItem->setIcon(QIcon("icons/mainIcon.png"));
	m_mainMenuItem->setText(tr("Główne"));
	m_mainMenuItem->setTextAlignment(Qt::AlignCenter);

	m_ordersMenuItem = new QListWidgetItem(m_listWidget);
	m_ordersMenuItem->setIcon(QIcon("icons/order.png"));
	m_ordersMenuItem->setText(tr("Zamówienia"));
	m_ordersMenuItem->setTextAlignment(Qt::AlignCenter);

	m_serviceMenuItem = new QListWidgetItem(m_listWidget);
	m_serviceMenuItem->setIcon(QIcon("icons/services.png"));
	m_serviceMenuItem->setText(tr("Usługi"));
	m_serviceMenuItem->setTextAlignment(Qt::AlignCenter);

	m_mailMenuItem = new QListWidgetItem(m_listWidget);
	m_mailMenuItem->setIcon(QIcon("icons/mail.png"));
	m_mailMenuItem->setText(tr("E-Mail"));
	m_mailMenuItem->setTextAlignment(Qt::AlignCenter);

	m_listWidget->addItem(m_mainMenuItem);
	m_listWidget->addItem(m_ordersMenuItem);
	m_listWidget->addItem(m_serviceMenuItem);
	m_listWidget->addItem(m_mailMenuItem);
}

//-----------------------------------------
// zmiana strony konfiguracji
void ConfigurationDialog::changePage(QListWidgetItem * current, QListWidgetItem * previous)
{
	if (!current)
		current = previous;

	m_stackedWidget->setCurrentIndex(m_listWidget->row(current));
}
