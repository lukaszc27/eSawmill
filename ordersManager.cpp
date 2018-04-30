#include "ordersManager.h"
#include <qlayout.h>
#include <qmessagebox.h>
#include <qmenu.h>
#include "addOrderDialog.h"


OrdersManager::OrdersManager(QWidget* parent)
	: QWidget(parent)
{
	setWindowIcon(QIcon("icons/order.png"));
	setWindowTitle(tr("Zarządzanie zamówieniami"));
	setMinimumSize(960, 480);

	// tworzenie dialogów
	m_dateFilterDialog = new DateFilterDialog(this);

	// tworzenie modeli prezentacji
	m_proxyModel			= new OrdersProxyModel(this);
	m_realizedFilterModel	= new RealizedOrdersFilter(this);
	m_model					= new OrdersModel(this);

	// tworzenie kontrolek oraz sktótów klawiszowych
	createWidgets();
	createShortcut();

	m_proxyModel->setSourceModel(m_model);
	m_realizedFilterModel->setSourceModel(m_proxyModel);
	m_treeView->setModel(m_realizedFilterModel);

	m_filterOption->addItems(m_model->headerLabels());	// dodajemy listę do comboBox-a

	connect(m_addButton, SIGNAL(clicked()), this, SLOT(addNewOrder()));
	connect(m_treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editOrder(QModelIndex)));
	connect(m_removeButton, SIGNAL(clicked()), m_model, SLOT(removeCheckedOrders()));
	connect(m_filterButton, SIGNAL(clicked()), this, SLOT(filterOrders()));
	connect(m_filterText, SIGNAL(returnPressed()), this, SLOT(filterOrders()));
	connect(m_dateFilterButton, SIGNAL(clicked()), this, SLOT(dateFilterOption()));
	connect(m_afterRealizeDate, SIGNAL(stateChanged(int)), m_proxyModel, SLOT(showOrdersAfterRealizeDays(int)));
	
	connect(m_realizedFilterChecked, SIGNAL(stateChanged(int)),
		m_realizedFilterModel, SLOT(setFilterEnable(int)));
}

//-------------------------------------------
// tworzenie używanych w oknie widgetów
// oraz rozmieszczenie ich w oknie
void OrdersManager::createWidgets()
{
	m_filterOption = new QComboBox(this);
	m_filterText = new QLineEdit(this);
	m_filterButton = new QPushButton(QIcon("icons/find.png"), tr("Szukaj"), this);
	m_addButton = new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), this);
	m_removeButton = new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), this);
	m_dateFilterButton = new QPushButton(tr("Filter"), this);
	m_treeView = new MyTreeView(this);
	m_optionsGroup = new QGroupBox(this);
	m_afterRealizeDate = new QCheckBox(tr("Zamówienia po terminie realizacji"), this);
	m_realizedFilterChecked = new QCheckBox(tr("Wyświetl zamówienia zrealizowane"), this);


	QPushButton* optionsButton = new QPushButton(tr("Operacje"), this);
	QMenu* optionsButtonMenu = new QMenu(this);
	optionsButtonMenu->addAction(QIcon("icons/selectAll.png"), tr("Zaznacz wszystko"), m_model, SLOT(selectAllOrders()));
	optionsButtonMenu->addAction(QIcon("icons/unselectAll.png"), tr("Odznacz wszystko"), m_model, SLOT(unselectAllOrders()));
	optionsButtonMenu->addSeparator();
	optionsButtonMenu->addAction(tr("Zrealizuj zamówienia"), m_model, SLOT(realizeCheckedOrders()));
	optionsButton->setMenu(optionsButtonMenu);

	// tworzymy grupę szybkich filtrów
	QHBoxLayout* optionsLayout = new QHBoxLayout(m_optionsGroup);
	optionsLayout->addWidget(m_afterRealizeDate);
	optionsLayout->addWidget(m_realizedFilterChecked);
	optionsLayout->addStretch(1);

	// ułożenie elementów wyszukiwania zamówienia
	QHBoxLayout* filterLayout = new QHBoxLayout;
	filterLayout->addWidget(m_filterOption);
	filterLayout->addWidget(m_filterText);
	filterLayout->addWidget(m_filterButton);

	// ułożenie przycisków
	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_addButton);
	buttonsLayout->addWidget(m_removeButton);
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(m_dateFilterButton);
	buttonsLayout->addWidget(optionsButton);
	buttonsLayout->addStretch(6);

	// główny layout okna
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(filterLayout);
	mainLayout->addWidget(m_treeView);
	mainLayout->addWidget(m_optionsGroup);
	mainLayout->addLayout(buttonsLayout);
}

//-------------------------------------------
// przypisuje skróty klawiszowe akcjom
void OrdersManager::createShortcut()
{
	m_addButton->setShortcut(QKeySequence("Ins"));
	m_removeButton->setShortcut(QKeySequence("Delete"));
}

//-------------------------------------------
// wczytuje zamówienie do edycji
void OrdersManager::editOrder(const QModelIndex & index)
{
	AddOrderDialog* dialog = new AddOrderDialog(this, true, index.data(Qt::UserRole).toInt());
	if (dialog->exec() == AddOrderDialog::Accepted)
	{
		m_model->addOrderToDatabase(index.data(Qt::UserRole).toInt(), dialog->contrahentId(), dialog->addDate(),
			dialog->endDate(), dialog->title(), 0, dialog->isDone(), dialog->rabat(), 
			dialog->notes(), dialog->priorytet(), dialog->vat(), dialog->price(), dialog->listOfItems(), dialog->articlesItems());
	}
}

//-------------------------------------------
// filtruje zamówienia według podanych kryteriów
void OrdersManager::filterOrders()
{
	if (m_filterText->text().isEmpty())
	{
		m_proxyModel->clear();
		m_proxyModel->invalidate();
		m_proxyModel->setFilterRegExp("");
	}
	else
	{
		QString pattern = QString("(%1)([A-Za-z0-9]*)").arg(m_filterText->text().toUpper());
		m_proxyModel->setFilterRegExp(pattern);
		m_proxyModel->setFilterKeyColumn(m_filterOption->currentIndex());
	}
}

//-------------------------------------------
// filtruje listę zamówień ze względu na daty
void OrdersManager::dateFilterOption()
{
	if (m_dateFilterDialog->exec() == DateFilterDialog::Accepted)
	{
		m_proxyModel->setFilterDate(m_dateFilterDialog->firstDate(), m_dateFilterDialog->lastDate());
		m_proxyModel->setDateFilterEnable(m_dateFilterDialog->filterEnabled());
		m_proxyModel->setDateFilterColumn(m_dateFilterDialog->filterColumn());
	}
}

//-------------------------------------------
// dodaje nowe zamówienie i odświeża listę
void OrdersManager::addNewOrder()
{
	AddOrderDialog* dialog = new AddOrderDialog(this);
	if (dialog->exec() == AddOrderDialog::Accepted)
	{
		m_model->addOrderToDatabase(0, dialog->contrahentId(), dialog->addDate(), dialog->endDate(),
			dialog->title(), 0, dialog->isDone(), dialog->rabat(), 
			dialog->notes(), dialog->priorytet(), dialog->vat(), dialog->price(), dialog->listOfItems(), dialog->articlesItems());
	}
}
