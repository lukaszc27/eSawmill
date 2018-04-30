#include "contrahentManager.h"
#include "addContrahentDialog.h"
#include "contrahentDelegate.h"
#include <qmessagebox.h>
#include <qmenu.h>

ContrahentManager::ContrahentManager(QWidget* parent, bool selectedMode)
	: QDialog(parent)
	, m_selectedMode(selectedMode)
{
	setWindowIcon(QIcon("icons/contrahents.png"));
	setWindowTitle(tr("Zarządzanie kontrahentami"));
	setMinimumSize(960, 480);

	//	if (!m_selectedMode)
	setAttribute(Qt::WA_DeleteOnClose);

	// tworzenie modeli
	m_proxyModel = new QSortFilterProxyModel(this);
	m_model = new ContrahentModel(this, selectedMode);

	// tworzymy widgety
	createWidgets();

	// ustawiamy model danych dla QTreeView
	m_proxyModel->setSourceModel(m_model);
	m_treeView->setModel(m_proxyModel);
	m_treeView->setItemDelegate(new ContrahentDelegate(this));
	m_model->loadFromDatabase();	// wczytujemy dane z bazy MySQL

	// ustawiamy dane z modelu do widoku
	m_filterOption->addItems(m_model->headerLabels());

	connect(m_addButton, SIGNAL(clicked()), this, SLOT(addNewContrahent()));
	connect(m_removeButton, SIGNAL(clicked()), m_model, SLOT(removeContrahent()));
	connect(m_filterButton, SIGNAL(clicked()), this, SLOT(filterContrahents()));
	connect(m_filterText, SIGNAL(returnPressed()), this, SLOT(filterContrahents()));

	// jeśli tryb wyboru łączymy dodatkowy sygnał
	if (m_selectedMode)
	{
		connect(m_treeView, SIGNAL(doubleClicked(QModelIndex)),
			this, SLOT(listDoubleClicked(QModelIndex)));
	}
}

//---------------------------------------------------
// tworzymy oraz ustawiamy widgety w oknie
void ContrahentManager::createWidgets()
{
	m_treeView		= new MyTreeView(this);
	m_filterOption	= new QComboBox(this);
	m_filterText	= new QLineEdit(this);
	m_filterButton	= new QPushButton(QIcon("icons/find.png"), tr("Szukaj"), this);
	m_addButton		= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), this);
	m_removeButton	= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), this);
	QPushButton* optionButton = new QPushButton(tr("Operacje"), this);

	// jeśli tryb wyboru blokujemy możliwość 
	if (m_selectedMode)
	{
//		m_addButton->setEnabled(false);
		m_removeButton->setEnabled(false);
		optionButton->setEnabled(false);
	}

	// przycisk operacji
	QMenu* optionButtonMenu = new QMenu(this);
	optionButtonMenu->addAction(QIcon("icons/selectAll.png"), tr("Zaznacz wszystko"),
		m_model, SLOT(selectAll()));

	optionButtonMenu->addAction(QIcon("icons/unselectAll.png"), tr("Odznacz wszystko"),
		m_model, SLOT(unselectAll()));

	optionButtonMenu->addSeparator();

	optionButtonMenu->addAction(tr("Oznacz jako dłużnika"), m_model, SLOT(markDebatorCheckedContrahents()));
	optionButtonMenu->addAction(tr("Usuń dłużnika/ów"), m_model, SLOT(removeCheckedDebators()));
	optionButton->setMenu(optionButtonMenu);

	// pasek filtrowania danych
	QHBoxLayout* filter = new QHBoxLayout;
	filter->addWidget(m_filterOption);
	filter->addWidget(m_filterText);
	filter->addWidget(m_filterButton);

	// pasek przycisków sterowania dialogiem
	QHBoxLayout* buttons = new QHBoxLayout;
	buttons->addWidget(m_addButton);
	buttons->addWidget(m_removeButton);
	buttons->addStretch(1);
	buttons->addWidget(optionButton);
	buttons->addStretch(6);

	// główny layout okna
	QVBoxLayout* main = new QVBoxLayout(this);
	main->addLayout(filter);
	main->addWidget(m_treeView);
	main->addLayout(buttons);
}

//----------------------------------------------
// wyszukiwanie kontrhenta po wybranych kategoriach
void ContrahentManager::filterContrahents()
{
	if (m_filterText->text().isEmpty())
	{
		// wyszukiwarka pusta pokazujemy wszystkich kontrahentów
		m_proxyModel->invalidate();
		m_proxyModel->clear();
		m_proxyModel->setFilterRegExp("");
	}
	else
	{
		// wyszukujemy kontrahenta
		QString pattern = QString("(%1)[A-Za-z0-9]*").arg(m_filterText->text().toUpper());
		m_proxyModel->setFilterRegExp(pattern);
		m_proxyModel->setFilterKeyColumn(m_filterOption->currentIndex());
	}
}

//----------------------------------------------
// podwujne kliknięcie w listę QTreeView
// w trybie wyboru (m_selectedMode = true)
// powoduje wyemitowanie sygnału wyboru kontrahenta
void ContrahentManager::listDoubleClicked(const QModelIndex& index)
{
	if (m_selectedMode)
	{
		emit contrahentSelected(index.data(Qt::UserRole).toInt());
		QDialog::accept();	// zamykamy okno oraz powiadamiamy o powodzeniu
	}
}

//----------------------------------------------
// dodawanie nowego kontrahenta do bazy danych
void ContrahentManager::addNewContrahent()
{
	AddContrahentDialog* dialog = new AddContrahentDialog(this);
	if (dialog->exec() == AddContrahentDialog::Accepted)
	{
		// pobieramy dane z dialogu i wprowadzamy do bazy
		m_model->addNewContrahent(dialog->companyName(), dialog->name(), dialog->surname(), dialog->nip(),
			dialog->regon(), dialog->pesel(), dialog->zipCode(), dialog->zipName(), dialog->city(),
			dialog->street(), dialog->houseNumber(), dialog->phone(), dialog->mail(), dialog->website());
	}
}
