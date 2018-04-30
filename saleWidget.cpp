#include "saleWidget.h"
#include "squaredDelegate.h"
#include <qlayout.h>
#include <qformlayout.h>
#include <qaction.h>
#include <qmenu.h>
#include <qfiledialog.h>
#include <qprintdialog.h>
#include <qdatetime.h>
#include <qsettings.h>
#include <qtextstream.h>
#include <qprinter.h>
#include <qinputdialog.h>
#include <qmessagebox.h>

#include "clientDocument.h"
#include "addSingleSquaredDialog.h"
#include "articleModel.h"
#include "articlesManager.h"


SaleWidget::SaleWidget(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle(tr("Sprzedaż"));
	setWindowIcon(QIcon("icons/sale.png"));

	createWidgets();
	createShortcut();

	// aktywujemy/dezaktywujemy odpowiednie pola
	changePayOptions(m_payOption->currentIndex());

	// ustawiamy bierzące daty
	m_dayToPay->setDate(QDate::currentDate());
	m_saleDay->setDate(QDate::currentDate());

	// pobieramy cene z konfiguracji
	QSettings settings;
	m_price->setValue(settings.value("squaredPrice").toDouble());

	// połączenia sygnałów ze slotami
	connect(m_removeButton, SIGNAL(clicked()), m_model, SLOT(removeCheckedItems()));
	connect(m_addButton, SIGNAL(clicked()), this, SLOT(addNewElement()));
	connect(m_printButton, SIGNAL(clicked()), this, SLOT(print()));
	connect(m_payOption, SIGNAL(currentIndexChanged(int)), this, SLOT(changePayOptions(int)));
	connect(m_numberOfDay, SIGNAL(valueChanged(int)), this, SLOT(changeDateToPay(int)));
	connect(m_model, SIGNAL(modelChanged()), this, SLOT(updateAllWidgets()));
	connect(m_rabat, SIGNAL(valueChanged(int)), m_model, SLOT(setRabat(int)));
	connect(m_price, SIGNAL(valueChanged(double)), m_model, SLOT(setPrice(double)));
	connect(m_duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateSelectedItem()));
	connect(m_vatCheck, SIGNAL(stateChanged(int)), this, SLOT(setVatEnabled(int)));
	connect(m_vatValue, SIGNAL(activated(QString)), this, SLOT(setVatValue(QString)));
	connect(m_addArticle, SIGNAL(clicked()), this, SLOT(addArticle()));
	connect(m_articleModel, SIGNAL(modelChanged()), this, SLOT(updateAllWidgets()));
	connect(m_articleFilterText, SIGNAL(textChanged(QString)), this, SLOT(filterArticlesItems(QString)));
}
//----------------------------------------
// tworzy widgety używane w oknie
void SaleWidget::createWidgets()
{
	m_contrahentGroup	= new QGroupBox(tr("Kontrahent"), this);
	m_saleGroup			= new QGroupBox(tr("Płatność"), this);
	m_tabWidget			= new QTabWidget(this);

	// dodawanie zakładek
	m_tabWidget->addTab(createElementTab(), tr("Elementy konstrukcyjne"));
	m_tabWidget->addTab(createArticleTab(), tr("Artykuły"));

	// grupa płatności
	m_saleDay		= new QDateEdit(this);
	m_dayToPay		= new QDateEdit(this);
	m_payOption		= new QComboBox(this);
	m_numberOfDay	= new QSpinBox(this);
	m_price			= new QDoubleSpinBox(this);
	m_rabat			= new QSpinBox(this);
	m_vatCheck		= new QCheckBox(tr("VAT"), this);
	m_vatValue		= new QComboBox(this);

	// ustawienie widgetów
	m_price->setMaximum(10000);
	m_saleDay->setCalendarPopup(true);
	m_dayToPay->setCalendarPopup(true);

	QStringList vatValues;
	vatValues << tr("0 %") << tr("3 %") << tr("5 %") << tr("8 %") << tr("23 %");
	m_vatValue->addItems(vatValues);

	m_vatCheck->setChecked(false);
	m_vatValue->setEnabled(false);

	// dodawanie danych do kontrolek
	QStringList payOption;
	payOption << tr("Gotówka") << tr("Przelew");
	m_payOption->addItems(payOption);

	// grupa kontrahent
	m_contrahentWidget = new ContrahentWidget(this);

	// ułożenie w grupie płatności
	QFormLayout* formLayout = new QFormLayout(m_saleGroup);
	formLayout->addRow(tr("Data sprzedaży"), m_saleDay);
	formLayout->addRow(tr("Płatność"), m_payOption);
	formLayout->addRow(tr("Termin płatności"), m_dayToPay);
	formLayout->addRow(tr("Liczba dni"), m_numberOfDay);
	formLayout->addRow(tr("Cena [zł/m<sup>3</sup>]"), m_price);
	formLayout->addRow(m_vatCheck, m_vatValue);
	formLayout->addRow(tr("Rabat [%]"), m_rabat);

	// uŁożenie w grupie kontrahent
	QHBoxLayout* contrahentLayout = new QHBoxLayout(m_contrahentGroup);
	contrahentLayout->addWidget(m_contrahentWidget);

	// układ główny okna
	QHBoxLayout* topLayout = new QHBoxLayout;
	topLayout->addWidget(m_contrahentGroup);
	topLayout->addWidget(m_saleGroup);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(m_tabWidget);
}

//----------------------------------------
// przypisuje skróty klawiszowe
void SaleWidget::createShortcut()
{
	m_addButton->setShortcut(QKeySequence("Ins"));
	m_removeButton->setShortcut(QKeySequence("Delete"));
}

//----------------------------------------
// tworzy widgety dla zakładki elementów
QWidget* SaleWidget::createElementTab()
{
	QWidget* widget		= new QWidget(this);
	m_treeView			= new QTreeView(widget);
	m_addButton			= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), widget);
	m_removeButton		= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), widget);
	m_optionButton		= new QPushButton(tr("Operacje"), widget);
	m_printButton		= new QPushButton(QIcon("icons/print.png"), tr("Drukuj"), widget);
	m_duplicateButton	= new QPushButton(tr("Powiel"), widget);
	m_planedOfMetric	= new QLabel(tr("<b>Strugane 0 m<sup>3</sup></b>"), this);
	m_sumOfMetric		= new QLabel(tr("<b>RAZEM 0 m<sup>3</sup></b>"), this);
	m_toPay				= new QLabel(tr("<b>Do zapłaty: 0 zł</b>"), this);

	// utworzenie modelu oraz połączenie go z widokiem
	m_model = new SquaredModel(this);
	m_treeView->setModel(m_model);
	m_treeView->setItemDelegate(new SquaredDelegate(this));
	m_treeView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
	m_model->setVatEnable(false);

	// menu dla przycisku operacje
	QMenu* optionMenu = new QMenu(this);
	optionMenu->addAction(QIcon("icons/selectAll.png"), tr("Zaznacz wszystko"),
		m_model, SLOT(selectAll()), QKeySequence("Ctrl+A"));
	optionMenu->addAction(tr("Odznacz wszystko"), m_model, SLOT(unselectAll()));
	optionMenu->addSeparator();
	optionMenu->addAction(QIcon("icons/import.png"), tr("Import"), this, SLOT(importFromXML()));
	optionMenu->addAction(QIcon("icons/export.png"), tr("Eksport"), this, SLOT(exportToXML()));
	m_optionButton->setMenu(optionMenu);

	// zmiana ustawien rozciągania etykiet
	m_planedOfMetric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_sumOfMetric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_toPay->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	// tworzenie layautów
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_addButton);
	buttonLayout->addWidget(m_removeButton);
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(m_optionButton);
	buttonLayout->addWidget(m_duplicateButton);
	buttonLayout->addWidget(m_printButton);
	buttonLayout->addStretch(6);

	QHBoxLayout* labelLayout = new QHBoxLayout;
	labelLayout->addStretch(6);
	labelLayout->addWidget(m_planedOfMetric, Qt::AlignRight);
	labelLayout->addStretch(1);
	labelLayout->addWidget(m_sumOfMetric, Qt::AlignRight);
	labelLayout->addStretch(1);
	labelLayout->addWidget(m_toPay, Qt::AlignRight);

	QVBoxLayout* mainElementLayout = new QVBoxLayout(widget);
	mainElementLayout->addWidget(m_treeView);
	mainElementLayout->addLayout(labelLayout);
	mainElementLayout->addLayout(buttonLayout);

	return widget;
}

//----------------------------------------
// tworzy widgety dla zakładki artykułów
QWidget* SaleWidget::createArticleTab()
{
	QWidget* widget			= new QWidget(this);
	m_articleView			= new QTreeView(widget);
	m_addArticle			= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), widget);
	m_removeArticle			= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), widget);
	m_articleFilterColumn	= new QComboBox(widget);
	m_articleFilterText		= new QLineEdit(widget);
	m_toPayLabel			= new QLabel(tr("<b>Do zapłaty: 0 zł</b>"), widget);

	m_toPayLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	// dodanie modelu do widoku
	m_articleProxyModel = new QSortFilterProxyModel(this);
	m_articleModel = new ArticleModel(this);
	m_articleProxyModel->setSourceModel(m_articleModel);
	m_articleView->setModel(m_articleProxyModel);
	m_articleView->setItemDelegate(new ArticleDelegate(this));

	// dodanie nazw kolumn do listy rozwijanej
	m_articleFilterColumn->addItems(m_articleModel->headerList());

	QHBoxLayout* filterLayout = new QHBoxLayout;
	filterLayout->addWidget(m_articleFilterColumn);
	filterLayout->addWidget(m_articleFilterText);

	QHBoxLayout* labelLayout = new QHBoxLayout;
	labelLayout->addStretch(1);
	labelLayout->addWidget(m_toPayLabel);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_addArticle);
	buttonsLayout->addWidget(m_removeArticle);
	buttonsLayout->addStretch(1);

	QVBoxLayout* mainLayout = new QVBoxLayout(widget);
	mainLayout->addLayout(filterLayout);
	mainLayout->addWidget(m_articleView);
	mainLayout->addLayout(labelLayout);
	mainLayout->addLayout(buttonsLayout);

	return widget;
}

//----------------------------------------
// importowanie elementów z pliku
void SaleWidget::importFromXML()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Import z pliku XML"),
		QDir::currentPath(), "*.xml\0*.*\0");

	if (!fileName.isEmpty())
		m_model->importFromXML(fileName);
}

//----------------------------------------
// eksport elementów do pliku XML
void SaleWidget::exportToXML()
{
	const QString fileName = QFileDialog::getSaveFileName(this, tr("Zapis do pliku XML"),
		QDir::currentPath(), "*.xml\0*.*\0");

	if (!fileName.isEmpty())
		m_model->exportToXML(fileName);
}

//----------------------------------------
// dodaje nowy element do listy
void SaleWidget::addNewElement()
{
	AddSingleSquaredDialog* dialog = new AddSingleSquaredDialog(this);
	if (dialog->exec() == AddSingleSquaredDialog::Accepted)
	{
		m_model->addItem(dialog->width(), dialog->height(), dialog->length(), dialog->count(), dialog->isPlaned());
	
		QSettings settings;
		if (settings.value("squared/fastAdd").toBool())
			addNewElement();
	}
}

//----------------------------------------
// aktywuje lub dezaktywuje niektóre widgety
// służące do płatności
void SaleWidget::changePayOptions(int index)
{
	switch (index)
	{
	case 0:		// gotówka
		m_dayToPay->setEnabled(false);
		m_numberOfDay->setEnabled(false);
		break;

	case 1:		// przelewy
		m_dayToPay->setEnabled(true);
		m_numberOfDay->setEnabled(true);
		break;
	}
}

//----------------------------------------
// dodanie liczby dni do daty płatności
void SaleWidget::changeDateToPay(int value)
{
	QDate date = m_saleDay->date();
	date = date.addDays(value);
	m_dayToPay->setDate(date);
}

//----------------------------------------
// drukuje rozliczenie (paragon)
void SaleWidget::print()
{
	QString other;
	QTextStream out(&other);

	// wyliczamy sumę do zapłaty
	QSettings settings;
	double toPay = m_model->sumOfMetric()*m_price->value() +
		m_model->planedOfMetric()*settings.value("planedPrice").toDouble();

	if (m_articleModel->listOfItems().size() > 0)
	{
		out << tr("<br /><center><h3>ARTYKUŁY</h3></center>");
		out << m_articleModel->itemsToHtml();
	}

/*	out << "<br />";
	out << "<center><table border=\"1\" style=\"font-size: 10px;\">";
	out << "<tr><th>" << tr("Nazwa") << "</th>"
		<< "<th>" << tr("Ilość<br>[szt]") << "</th>"
		<< "<th>" << tr("Cena<br>[zł/m<sup>3</sup>]") << "</th>";

	if (m_model->vatEnabled())
	{
		out << "<th>" << tr("Wartość netto<br>[zł]") << "</th>"
			<< "<th>" << tr("VAT<br>[%]") << "</th>"
			<< "<th>" << tr("Wartość brutto<br>[zł]") << "</th></tr>";
	}
	else
	{
		out << "<th>" << tr("Wartość<br>[zł]") << "</th></tr>";
	}

	if (m_model->vatEnabled())
	{
		double netto = m_model->sumOfMetric()*m_price->value();
		double vat = 100 + m_model->vatValue();
		double brutto = (netto * vat) / 100;

		out << "<tr><td>" << tr("Elementy") << "</td>"
			<< "<td>" << m_model->sumOfMetric() << "</td>"
			<< "<td>" << m_price->value() << "</td>"
			<< "<td>" << netto << "</td>"
			<< "<td>" << m_model->vatValue() << "%</td>"
			<< "<td>" << brutto << "</td></tr>";

		netto = m_model->planedOfMetric() * settings.value("planedPrice").toDouble();
		brutto = (netto * vat) / 100;

		out << "<tr><td>" << tr("Struganie") << "</td>"
			<< "<td>" << m_model->planedOfMetric() << "</td>"
			<< "<td>" << settings.value("planedPrice").toDouble() << "</td>"
			<< "<td>" << netto << "</td>"
			<< "<td>" << m_model->vatValue() << "%</td>"
			<< "<td>" << brutto << "</td></tr>";

		if (m_articleModel->listOfItems().size() > 0)
		{
			out << "<tr><td>" << tr("Artykuły") << "</td>"
				<< "<td></td>"
				<< "<td></td>"
				<< "<td></td>"
				<< "<td></td>"
				<< "<td>" << m_articleModel->priceToPay() << "</td></tr>";
		}

		netto = m_model->price() + m_articleModel->priceToPay();
		brutto = (netto * vat) / 100;

		out << "<tr><td colspan=\"3\"><b>" << tr("RAZEM") << "</b></td>"
			<< "<td>" << netto << "</td>"
			<< "<td>" << m_model->vatValue() << "%</td>"
			<< "<td>" << brutto << "</td></tr>";
	}
	else
	{
		out << "<tr><td>" << tr("Elementy") << "</td>"
			<< "<td>" << m_model->sumOfMetric() << "m<sup>3</sup></td>"
			<< "<td>" << m_model->price() << tr("zł") << "</td>"
			<< "<td>" << m_model->sumOfMetric() * m_model->price() << "</td></tr>";

		out << "<tr><td>" << tr("Strugane") << "</td>"
			<< "<td>" << m_model->planedOfMetric() << "m<sup>3</sup></td>"
			<< "<td>" << settings.value("planedPrice").toString() << tr("zł") << "</td>"
			<< "<td>" << m_model->planedOfMetric() * settings.value("planedPrice").toDouble() << "</td></tr>";

		if (m_articleModel->listOfItems().size() > 0)
		{
			out << "<tr><td>" << tr("Artykuły") << "</td>"
				<< "<td></td>"
				<< "<td></td>"
				<< "<td>" << m_articleModel->priceToPay() << "</td></tr>";
		}

		out << "<tr><td colspan=\"3\"><b><p align=\"right\">" << tr("RAZEM") << "</p></b></td>"
			<< "<td><b>" << m_model->price() + m_articleModel->priceToPay() << "</b></td></tr>";
	}

	out << "</table></center><br>";*/
	
	double netto = m_model->price() + m_articleModel->priceToPay();
	double vat = 100 + m_model->vatValue();
	double brutto = (netto*vat) / 100;

	out << "<br/>";
	out << "<h3 style=\"padding:0px; margin: 0px;\">" << tr("Do zapłaty: ");
	if (m_model->vatEnabled()) out << brutto;
	else out << netto;
	out << tr("zł") << "</h3><br>";

	out << tr("Płatność: ") << m_payOption->currentText() << "<br>";
	if (m_payOption->currentIndex() == 1)	// płatność przelew
	{
		out << "Bank: " << settings.value("app/bank").toString() << "<br>";
		out << "Numer konta: " << settings.value("app/bankNumber").toString() << "<br>";
		out << tr("Termin płatności: ") << m_dayToPay->date().toString("dd.MM.yyyy") << "<br>";
	}
	if (m_model->rabat())
		out << tr("Rabat: ") << m_model->rabat() << "%<br>";

	// przygotowujemy dokument do druku
	ClientDocument* document = new ClientDocument(this);
	document->setDocumentDate(m_saleDay->date());
	document->setDocumentTitle(tr("Sprzedaż"));
	document->prepareToPrint(m_contrahentWidget->contrahentAddress(), 
		m_model->listOfItems().size() > 0 ? m_model->itemsToHtml() : "", other);

	QPrinter* printer = new QPrinter;
	QPrintDialog* dialog = new QPrintDialog(printer, this);
	if (dialog->exec() == QPrintDialog::Accepted)
		document->print(printer);
}

//----------------------------------------
// aktualizujemy wszystkie widgety
// w zależności od potrzeb modelu
void SaleWidget::updateAllWidgets()
{
	QSettings settings;

	double netto = m_model->price();
	double vat = 100 + m_model->vatValue();
	double brutto = (netto*vat) / 100;

	m_planedOfMetric->setText(tr("<b>Strugane: %1 m<sup>3</sup></b>").arg(m_model->planedOfMetric()));
	m_sumOfMetric->setText(tr("<b>RAZEM: %1 m<sup>3</sup></b>").arg(m_model->sumOfMetric()));
	m_toPay->setText(tr("<b>Do zapłaty: %1 zł</b>").arg(m_model->vatEnabled() ? brutto : netto));
	m_toPayLabel->setText(tr("<b>Do zapłaty: %1 zł</b>").arg(m_articleModel->priceToPay()));
}

//----------------------------------------
// duplikuje zaznaczony element
void SaleWidget::duplicateSelectedItem()
{
	QItemSelectionModel* selectionModel = m_treeView->selectionModel();
	QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

	m_model->addItem(selectedIndexes.at(0).data().toDouble(),
		selectedIndexes.at(1).data().toDouble(),
		selectedIndexes.at(2).data().toDouble(),
		selectedIndexes.at(3).data().toDouble(),
		selectedIndexes.at(4).data(Qt::EditRole).toBool());
}

//----------------------------------------
// aktywuje lub dezaktywuje funkcje
// dodawania VAT-u
void SaleWidget::setVatEnabled(int state)
{
	if (state)
	{
		m_model->setVatEnable(true);
		m_vatValue->setEnabled(true);
	}
	else
	{
		m_model->setVatEnable(false);
		m_vatValue->setEnabled(false);
	}
}

//----------------------------------------
// nadaje wartość VAT-u do modelu
void SaleWidget::setVatValue(const QString & value)
{
	QString val = value;
	val.remove(" %");
	m_model->setVatValue(val.toInt());
}

//----------------------------------------
// wywołuje dialog wyboru artykułu
void SaleWidget::addArticle()
{
	ArticlesManager* manager = new ArticlesManager(this, true);
	connect(manager, SIGNAL(articleSelected(int)), this, SLOT(addArticle(int)));

	manager->exec();
}

//----------------------------------------
// dodaje wybrany artykuł do listy
void SaleWidget::addArticle(int id)
{
	double count = QInputDialog::getDouble(this, tr("Dodawanie nowego artykułu"),
		tr("Ilość"), 1);

	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen() && db.isValid())
	{
		QSqlQuery query;
		query.prepare("SELECT idMagazynu, nazwa, cenaNetto, cenaBrutto, "
			"vat, jm, cenaDetaliczna, marza FROM artykuly WHERE idArtykulu = ?");

		query.bindValue(0, id);
		if (query.exec())
		{
			while (query.next())
				m_articleModel->addItem(query.value("nazwa").toString(),
					query.value("jm").toString(), query.value("cenaNetto").toDouble(),
					query.value("cenaBrutto").toDouble(), query.value("vat").toInt(),
					query.value("idMagazynu").toInt(), query.value("cenaDetaliczna").toDouble(),
					query.value("marza").toDouble(), count, id);
		}
		else
		{
			QMessageBox::critical(this, tr("Błąd"),
				query.lastError().text());

			return;
		}
	}
}

void SaleWidget::filterArticlesItems(const QString & text)
{
	if (!text.isEmpty())
	{
		QString pattern = tr("(%1)[A-Za-z0-9]*").arg(text.toUpper());
		m_articleProxyModel->setFilterRegExp(pattern);
		m_articleProxyModel->setFilterKeyColumn(m_articleFilterColumn->currentIndex());
	}
	else
	{
		m_articleProxyModel->setFilterRegExp("");
		m_articleProxyModel->setFilterKeyColumn(0);
		m_articleProxyModel->invalidate();
	}
}
