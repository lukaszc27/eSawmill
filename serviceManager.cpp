#include "serviceManager.h"
#include "addServiceDialog.h"
#include <qlayout.h>
#include <qmessagebox.h>
#include <qmenu.h>

ServiceManager::ServiceManager(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle(tr("Zarządzanie usługami"));
	setWindowIcon(QIcon("icons/services.png"));
	setMinimumSize(960, 480);

	m_dateFilterDialog = new DateFilterDialog(this);

	createWidgets();
	createShortcut();

	connect(m_addButton, SIGNAL(clicked()), this, SLOT(addNewService()));
	connect(m_treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editService(QModelIndex)));
	connect(m_filterButton, SIGNAL(clicked()), this, SLOT(filterService()));
	connect(m_filterText, SIGNAL(returnPressed()), this, SLOT(filterService()));
	connect(m_dateFilterButton, SIGNAL(clicked()), this, SLOT(filterServiceByDate()));
}

//-------------------------------------------
// tworzy dodatkowe widgety wykorzystynwe
// w zarządzaniu usługami
void ServiceManager::createWidgets()
{
	m_filterOption	= new QComboBox(this);
	m_filterText	= new QLineEdit(this);
	m_filterButton	= new QPushButton(QIcon("icons/find.png"), tr("Szukaj"), this);
	m_treeView		= new MyTreeView(this);
	m_addButton		= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), this);
	m_removeButton	= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), this);
	m_dateFilterButton = new QPushButton(tr("Filter"), this);

	// przypisanie modelu
	m_model = new ServiceModel(this);
	m_dateProxyModel = new OrdersProxyModel(this);

	m_dateProxyModel->setSourceModel(m_model);
	m_treeView->setModel(m_dateProxyModel);

	connect(m_removeButton, SIGNAL(clicked()), m_model, SLOT(removeCheckedItems()));

	// przycisk operacje
	QPushButton* optionButton = new QPushButton(tr("Operacje"), this);
	QMenu* optionButtonMenu = new QMenu(this);
	optionButtonMenu->addAction(QIcon("icons/selectAll.png"), tr("Zaznacz wszystko"), m_model, SLOT(selectAll()));
	optionButtonMenu->addAction(tr("Odznacz wszystko"), m_model, SLOT(unselectAll()));
	optionButtonMenu->addSeparator();
	optionButtonMenu->addAction(tr("Zrealizuj usługe"), m_model, SLOT(realiseCheckedItems()));
	optionButton->setMenu(optionButtonMenu);

	// pobranie nazw nagłówków i dodanie do listy w wyszukiwarce
	m_filterOption->addItems(m_model->headerLists());

	QHBoxLayout* filterLayout = new QHBoxLayout;
	filterLayout->addWidget(m_filterOption);
	filterLayout->addWidget(m_filterText);
	filterLayout->addWidget(m_filterButton);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_addButton);
	buttonsLayout->addWidget(m_removeButton);
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(m_dateFilterButton);
	buttonsLayout->addWidget(optionButton);
	buttonsLayout->addStretch(6);

	// główny layout
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(filterLayout);
	mainLayout->addWidget(m_treeView);
	mainLayout->addLayout(buttonsLayout);
}

//-------------------------------------------
// przypisuje sktóty klawiszowe
void ServiceManager::createShortcut()
{
	m_addButton->setShortcut(QKeySequence("Ins"));
	m_removeButton->setShortcut(QKeySequence("Delete"));
}

//-------------------------------------------
// wczytuje usługe do aktualizacji
void ServiceManager::editService(const QModelIndex & index)
{
	AddServiceDialog* dialog = new AddServiceDialog(this, true, index.data(Qt::UserRole).toInt());
	if (dialog->exec() == QDialog::Accepted)
	{
		m_model->addService(dialog->title(), dialog->addDate(), dialog->endDate(),
			dialog->isDone(), dialog->rabat(), dialog->notes(), dialog->roundItemsOfList(),
			dialog->squaredItemsOfList(),dialog->articlesOfList(), dialog->contrahentId(), index.data(Qt::UserRole).toInt(), 
			dialog->priorytet(), dialog->vat());
	}
}

//-------------------------------------------
// wyszukuje usługe po podanych kryteriach
void ServiceManager::filterService()
{
	if (m_filterText->text().isEmpty())
	{
		// czyścimy wszystkie filtry
		m_dateProxyModel->clear();
		m_dateProxyModel->setFilterRegExp("");
		m_dateProxyModel->setFilterKeyColumn(0);
		m_dateProxyModel->invalidate();
	}
	else
	{
		QString pattern = tr("(%1)([A-Za-z0-9.]*)").arg(m_filterText->text().toUpper());
		m_dateProxyModel->setFilterKeyColumn(m_filterOption->currentIndex());
		m_dateProxyModel->setFilterRegExp(pattern);
	}
}

//-------------------------------------------
// filtruje usługi według podanych karetogii
void ServiceManager::filterServiceByDate()
{
	if (m_dateFilterDialog->exec() == DateFilterDialog::Accepted)
	{
		m_dateProxyModel->setFilterDate(m_dateFilterDialog->firstDate(), m_dateFilterDialog->lastDate());
		m_dateProxyModel->setDateFilterEnable(m_dateFilterDialog->filterEnabled());
		m_dateProxyModel->setDateFilterColumn(m_dateFilterDialog->filterColumn());
	}
}

//-------------------------------------------
// dodaje nowa usługe do listy
void ServiceManager::addNewService()
{
	AddServiceDialog* dialog = new AddServiceDialog(this);
	if (dialog->exec() == AddServiceDialog::Accepted)
	{
		m_model->addService(dialog->title(), dialog->addDate(), dialog->endDate(),
			dialog->isDone(), dialog->rabat(), dialog->notes(), dialog->roundItemsOfList(), 
			dialog->squaredItemsOfList(),dialog->articlesOfList(), dialog->contrahentId(), 0, dialog->priorytet(), dialog->vat());
	}
}
