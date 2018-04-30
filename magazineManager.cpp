#include "magazineManager.h"
#include <qlayout.h>
#include "addMagazineDialog.h"


MagazineManager::MagazineManager(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle(tr("Zarządzanie magazynami artykułów"));
	setMinimumSize(QSize(500, 300));
	createWidgets();

	connect(m_addButton, SIGNAL(clicked()), this, SLOT(addMagazine()));
	connect(m_treeView, SIGNAL(doubleClicked(QModelIndex)),
		this, SLOT(listDoubleClicked(QModelIndex)));
	connect(m_removeButton, SIGNAL(clicked()), m_model, SLOT(removeSelectedItems()));
}

//----------------------------------------
// tworzy widgety dodatkowe w oknie
void MagazineManager::createWidgets()
{
	m_treeView		= new QTreeView(this);
	m_addButton		= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), this);
	m_removeButton	= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), this);

	m_model = new MagazineModel(this);
	m_treeView->setModel(m_model);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_addButton);
	buttonsLayout->addWidget(m_removeButton);
	buttonsLayout->addStretch(1);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(m_treeView);
	mainLayout->addLayout(buttonsLayout);
}

//----------------------------------------
// dokonuje aktualizacji wybranego elementu
// po podwójnym kliknięciu
void MagazineManager::listDoubleClicked(const QModelIndex& index)
{
	AddMagazineDialog* dialog = new AddMagazineDialog(this, true, index.data(Qt::UserRole).toInt());
	if (dialog->exec() == AddMagazineDialog::Accepted)
		m_model->addMagazine(dialog->name(), dialog->description(), index.data(Qt::UserRole).toInt());
}

//----------------------------------------
// dodaje nowy magazyn do listy
void MagazineManager::addMagazine()
{
	AddMagazineDialog* dialog = new AddMagazineDialog(this);
	if (dialog->exec() == AddMagazineDialog::Accepted)
		m_model->addMagazine(dialog->name(), dialog->description());
}
