#include "articlesManager.h"
#include "addArticleDialog.h"
#include <qlayout.h>
#include <qmessagebox.h>
#include <qlabel.h>


ArticlesManager::ArticlesManager(QWidget* parent, bool selectedMode)
	: QDialog(parent)
	, m_selectedMode(selectedMode)
{
	m_db = QSqlDatabase::database();

	setWindowTitle(tr("Zarządzanie artykułami"));
	setMinimumSize(800, 480);
	createWidgets();

	if (selectedMode)
	{
		m_removeButton->setEnabled(false);
		m_duplicateButton->setEnabled(false);
		m_editButton->setEnabled(false);

		// zmieniamy tytuł okna
		setWindowTitle(tr("Wybór artykułu"));
	}

	connect(m_addButton, SIGNAL(clicked()), this, SLOT(addArticle()));
	connect(m_treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editArticle(QModelIndex)));
	connect(m_editButton, SIGNAL(clicked()), this, SLOT(editArticleButtonClicked()));
	connect(m_duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateArticle()));
	connect(m_removeButton, SIGNAL(clicked()), m_model, SLOT(removeSelectedArticle()));
	connect(m_filterText, SIGNAL(returnPressed()), this, SLOT(filterArticle()));
	connect(m_filterMagazine, SIGNAL(activated(int)), this, SLOT(magazineActivated(int)));
}

//-----------------------------------------------
// tworzenie widgetów
void ArticlesManager::createWidgets()
{
	m_addButton = new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), this);
	m_removeButton = new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), this);
	m_editButton = new QPushButton(tr("Edytuj"), this);
	m_duplicateButton = new QPushButton(tr("Powiel"), this);
	m_treeView = new MyTreeView(this);
	m_filterText = new QLineEdit(this);
	m_filterColumn = new QComboBox(this);
	m_filterMagazine = new QComboBox(this);

	// przypisanie modelu
	m_model = new ArticleManagerModel(this);
	m_proxyModel = new QSortFilterProxyModel(this);
	m_proxyModel->setSourceModel(m_model);
	m_treeView->setModel(m_proxyModel);

	// pobieramy z bazy listę magazynów
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery q;
		if (q.exec("select idMagazynu, nazwa from artykuly_magazyn"))
		{
			while (q.next())
				m_filterMagazine->addItem(q.value(1).toString(), q.value(0).toInt());
		}
		else
		{
			QMessageBox::critical(this, tr("Błąd"),
				q.lastError().text());

			return;
		}
	}

	// ustawiamy nazwy kolumn w comboBox-ie
	m_filterColumn->addItems(m_model->headerList());

	// tworzymy layout dla filtra
	QHBoxLayout* filterLayout = new QHBoxLayout;
	filterLayout->addWidget(m_filterColumn);
	filterLayout->addWidget(m_filterText);
	filterLayout->addWidget(m_filterMagazine);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_addButton);
	buttonsLayout->addWidget(m_removeButton);
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(m_editButton);
	buttonsLayout->addWidget(m_duplicateButton);
	buttonsLayout->addStretch(6);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(filterLayout);
	mainLayout->addWidget(m_treeView);
	mainLayout->addLayout(buttonsLayout);
}

//-----------------------------------------------
// tworzy sktóty klawiszwe
void ArticlesManager::createShortcut()
{
	m_addButton->setShortcut(QKeySequence("Ins"));
	m_removeButton->setShortcut(QKeySequence("Del"));
}

//-----------------------------------------------
// edycja zaznaczonego artykułu
void ArticlesManager::editArticle(const QModelIndex & index)
{
	if (!m_selectedMode)
	{
		AddArticleDialog* dialog = new AddArticleDialog(this, index.data(Qt::UserRole).toInt(), true);
		if (dialog->exec() == AddArticleDialog::Accepted)
		{
			m_model->addItem(dialog->name(), dialog->jm(), dialog->nettoPrice(), dialog->bruttoPrice(),
				dialog->vat(), dialog->magazineId(), dialog->detalPrice(), dialog->marza(), dialog->articleId());
		}
	}
	else
	{
		emit articleSelected(index.data(Qt::UserRole).toInt());
		QDialog::accept();
	}
}

//-----------------------------------------------
// edycja artykułu po naciśnięciu przyciku edycji
void ArticlesManager::editArticleButtonClicked()
{
	editArticle(m_treeView->currentIndex());
}

//-----------------------------------------------
// duplikuje zaznaczony artykuł
void ArticlesManager::duplicateArticle()
{
	QItemSelectionModel* selectionModel = m_treeView->selectionModel();
	QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
	
	m_model->addItem(selectedIndexes.at(0).data().toString(),
		selectedIndexes.at(1).data().toString(), selectedIndexes.at(2).data().toDouble(),
		selectedIndexes.at(3).data().toDouble(), selectedIndexes.at(4).data().toDouble(),
		m_filterMagazine->itemData(m_filterMagazine->currentIndex()).toInt(),
		selectedIndexes.at(6).data().toDouble(), selectedIndexes.at(5).data().toDouble());
}

//-----------------------------------------------
// filtruje listę artykułów po zadanych kryteriach
void ArticlesManager::filterArticle()
{
	if (m_filterText->text().isEmpty())
	{
		m_proxyModel->setFilterRegExp("");
		m_proxyModel->setFilterKeyColumn(0);
		m_proxyModel->invalidate();
	}
	else
	{
		QString pattern = tr("(%1)[A-Za-z0-9]*").arg(m_filterText->text().toUpper());
		m_proxyModel->setFilterRegExp(pattern);
		m_proxyModel->setFilterKeyColumn(m_filterColumn->currentIndex());
	}
}

//-----------------------------------------------
// wczytuje elementy po wybraniu magazynu
void ArticlesManager::magazineActivated(int index)
{
	m_model->loadFromDatabase(m_filterMagazine->currentData(Qt::UserRole).toInt());
}

//-----------------------------------------------
// dodaje nowy artykuł do bazy
void ArticlesManager::addArticle()
{
	AddArticleDialog* dialog = new AddArticleDialog(this);
	if (dialog->exec() == AddArticleDialog::Accepted)
	{
		m_model->addItem(dialog->name(), dialog->jm(), dialog->nettoPrice(), dialog->bruttoPrice(),
			dialog->vat(), dialog->magazineId(), dialog->detalPrice(), dialog->marza());
	}
}
