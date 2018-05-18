#include "addOrderDialog.h"
#include <qlayout.h>
#include <qformlayout.h>
#include <qmenu.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qsettings.h>
#include <qtextstream.h>
#include <qdatetime.h>
#include <qtabwidget.h>
#include <qinputdialog.h>

#include "addGroupSquaredDialog.h"
#include "squaredDelegate.h"
#include "clientDocument.h"
#include "addSingleSquaredDialog.h"
#include "articlesManager.h"
#include "articleDelegate.h"
#include "SmtpMime.h"


AddOrderDialog::AddOrderDialog(QWidget* parent, bool updateMode, int id)
	: QDialog(parent)
	, m_currentId(id)
	, m_updateMode(updateMode)
{
	// ustawienie ikony głownej okna
	setWindowIcon(QIcon("icons/order.png"));

	if(!m_updateMode)
		setWindowTitle(tr("Dodawanie nowego zamówienia"));
	else setWindowTitle(tr("Aktualizacja zamówienia"));

	createWidgets();
	createShortcut();

	connect(m_printButton, SIGNAL(clicked()), this, SLOT(print()));
	connect(m_removeButton, SIGNAL(clicked()), m_model, SLOT(removeCheckedItems()));
	connect(m_model, SIGNAL(modelChanged()), this, SLOT(updateAllWidgets()));
	connect(m_discountEdit, SIGNAL(valueChanged(int)), m_model, SLOT(setRabat(int)));
	connect(m_acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_rejectButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_sizeFilterGroup, SIGNAL(clicked(bool)), m_elementsProxyModel, SLOT(setEnable(bool)));
	connect(m_rangeFilterButton, SIGNAL(clicked()), this, SLOT(filterByRange()));
	connect(m_duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateSelectedItem()));
	connect(m_vatEnable, SIGNAL(stateChanged(int)), this, SLOT(vatEnabled(int)));
	connect(m_vatEdit, SIGNAL(activated(QString)), this, SLOT(activatedVatValue(QString)));
	connect(m_price, SIGNAL(valueChanged(double)), m_model, SLOT(setPrice(double)));
	connect(m_addArticle, SIGNAL(clicked()), this, SLOT(addNewArticle()));
	connect(m_articleModel, SIGNAL(modelChanged()), this, SLOT(updateAllWidgets()));
	connect(m_removeArticle, SIGNAL(clicked()), m_articleModel, SLOT(removeCheckedItem()));
	connect(m_mailSendButton, SIGNAL(clicked()), this, SLOT(sendByMailButton()));

	connect(m_lengthFilterGroup, &QGroupBox::clicked,
			m_elementsProxyModel, &ElementsFilterModel::setLengthFilterEnable);

	connect(m_filterLengthButton, &QPushButton::clicked, this, &AddOrderDialog::filterByLength);

	// wczytujemy dane jeśli otwarto w trybie aktualizacji
	if (m_updateMode)
	{
		// tryb aktualizacji, wczytujemy dane z bazy
		if (!loadOrderFromDatabase())
		{
			QMessageBox::critical(this, tr("Błąd"), 
				tr("Podczas odczytu danych z bazy MySQL pojawiły się nieoczekiwane błędy :("));
			return;
		}
		if (!m_articleModel->loadItemsFromOrder(id))
		{
			QMessageBox::critical(this, tr("Błąd"),
				tr("Podczas odczytu zapisanych artykułów wystąpiły błędy"));

			return;
		}

		// blokowanie możliwości edycji
		m_contrahentGroup->setEnabled(false);	// brak możliwości edycji kontrahenta
		m_addDateEdit->setEnabled(false);		// brak możliwości edycji daty dodania zamówienia

		// i jeśli zamówienie zostało już zrealizowane
		// blokujemy wszystkie pola pozostawiając tylko listę elementów do odczytu
		// i możliwości eksportu oraz drukowania
		if (isDone())
		{
			m_contrahentGroup->setEnabled(false);
			m_orderGroup->setEnabled(false);
			m_addButton->setEnabled(false);
			m_removeButton->setEnabled(false);
			m_duplicateButton->setEnabled(false);
			m_model->readOnly(true);
			m_notesEdit->setReadOnly(true);
			m_addArticle->setEnabled(false);
			m_removeArticle->setEnabled(false);
			m_articleModel->setReadOnly(true);
		}
	}
	else
	{
		// tryb dodwania nowego zamówienia
		// ustawiamy aktualne daty
		m_addDateEdit->setDate(QDate::currentDate());
		m_endDateEdit->setDate(nearDate());
		m_vatEnable->setChecked(false);
	}
}

//--------------------------------------------------
// pobiera z bazy oraz ustawia najbliższą
// możliwą date terminu realizacji zamówienia
QDate AddOrderDialog::nearDate()
{
	QSqlDatabase db = QSqlDatabase::database();
	QDate date;

	if (db.isOpen() && db.isValid())
	{
		QSqlQuery q;
		if (q.exec("select max(terminRealizacji) from zamowienia"))
		{
			// zapytanie wykonano, pobieramy date
			if (q.first())
				date = q.value(0).toDate();
		}
		else date = QDate::currentDate();
	}
	date = date.addDays(3);
	return date;
}

//-----------------------------------------------
// wczytuje zamówienie z bazy
// umożliwiając jego edycje
bool AddOrderDialog::loadOrderFromDatabase()
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen() && db.isValid())
	{
		// połączenie z bazą danych nawiązane
		QSqlQuery query;
		query.prepare("select opis, dataPrzyjecia, terminRealizacji, naddatek, kontrahentId, "
			"zrealizowane, rabat, uwagi, priorytet, vat, cena "
			"from zamowienia where zamowienieId = ?");

		query.bindValue(0, m_currentId);
		if (query.exec())
		{
			// zapytanie się powiodło
			if (query.first())
			{
				m_titleEdit->setText(query.value(0).toString());
				m_addDateEdit->setDate(query.value(1).toDate());
				m_endDateEdit->setDate(query.value(2).toDate());
				// pominięty naddatek chwilowo
				m_contrahentWidget->loadContrahent(query.value(4).toInt());
				m_model->loadFromOrder(m_currentId);
				m_done->setChecked(query.value(5).toBool());
				m_discountEdit->setValue(query.value(6).toInt());	// zapisanie wielkości rabatu w oknie
				m_notesEdit->document()->setPlainText(query.value(7).toString());
				m_priotytetEdit->setCurrentIndex(query.value(8).toInt());
				m_price->setValue(query.value(10).toDouble());

				if (query.value(9).toInt() > 0)
				{
					QString vatText = query.value(9).toString();
					m_vatEnable->setChecked(true);
					m_vatEdit->setCurrentText(tr("%1 %").arg(vatText));
					m_model->setVatEnable(true);
					m_model->setVatValue(vatText.toInt());
				}
				else
				{
					m_model->setVatEnable(false);
					m_model->setVatValue(0);
				}

				// ustawienie rabatu w modelu
				m_model->setRabat(query.value(6).toInt());
				m_model->setPrice(query.value(10).toDouble());
				
				return true;
			}
		}
	}

	return false;
}

//-----------------------------------------------
// tworzy główne ułożenie widgetów w oknie
// oparte o kontener QTabWidget
void AddOrderDialog::createWidgets()
{
	QTabWidget* tabWidget = new QTabWidget(this);
	m_printButton		= new QPushButton(QIcon("icons/print.png"), tr("Drukuj"), this);
	m_mailSendButton	= new QPushButton(QIcon("icons/mail.png"), tr("Wyślij przez e-mail"), this);
	m_acceptButton		= new QPushButton(QIcon("icons/ok.png"), tr("OK"), this);
	m_rejectButton		= new QPushButton(QIcon("icons/cancel.png"), tr("Anuluj"), this);
	m_toPayLabel		= new QLabel(tr("<h3>Razem do zapłaty: 0 zł</h3>"), this);

	m_toPayLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch(2);
	buttonsLayout->addWidget(m_printButton);
	buttonsLayout->addWidget(m_mailSendButton);
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(m_acceptButton);
	buttonsLayout->addWidget(m_rejectButton);

	tabWidget->addTab(mainWidget(), tr("Ogólne"));
	tabWidget->addTab(elementsWidget(), tr("Elementy"));
	tabWidget->addTab(articleWidget(), tr("Artykuły"));

	QHBoxLayout* toPayLayout = new QHBoxLayout;
	toPayLayout->addStretch(1);
	toPayLayout->addWidget(m_toPayLabel);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(tabWidget);
	mainLayout->addLayout(toPayLayout);
	mainLayout->addSpacing(24);
	mainLayout->addLayout(buttonsLayout);
}

//-----------------------------------------------
// przypisuje odpowiednie sktóry klawiszowe
// odpowiednim akcjom
void AddOrderDialog::createShortcut()
{
	m_printButton->setShortcut(QKeySequence("Ctrl+P"));
	m_removeButton->setShortcut(QKeySequence("Delete"));
	m_addArticle->setShortcut(QKeySequence("Ins"));
	m_removeArticle->setShortcut(QKeySequence("Del"));

	m_addArticle->setShortcut(QKeySequence("Ins"));
	m_removeArticle->setShortcut(QKeySequence("Del"));

	m_acceptButton->setShortcut(QKeySequence("F10"));
	m_rejectButton->setShortcut(QKeySequence("ESC"));
}

//-----------------------------------------------
// tworzy widgety dla głównej zakładki
QWidget* AddOrderDialog::mainWidget()
{
	QWidget* widget = new QWidget;

	m_contrahentGroup	= new QGroupBox(tr("Kontrahent"), widget);
	m_orderGroup		= new QGroupBox(tr("Dane zamówienia"), widget);
	m_notesGroup		= new QGroupBox(tr("Uwagi"), widget);

	// widgety wprowadzania danych
	m_contrahentWidget	= new ContrahentWidget(widget);
	m_addDateEdit		= new QDateEdit(widget);
	m_endDateEdit		= new QDateEdit(widget);
	m_done				= new QCheckBox(widget);
	m_titleEdit			= new QLineEdit(widget);
	m_discountEdit		= new QSpinBox(widget);
	m_notesEdit			= new QTextEdit(widget);
	m_priotytetEdit		= new QComboBox(widget);
	m_vatEdit			= new QComboBox(widget);
	m_vatEnable			= new QCheckBox(tr("VAT"), widget);
	m_price				= new QDoubleSpinBox(this);

	m_price->setMaximum(10000);

	QStringList priotytetName;
	priotytetName << tr("Niski") << tr("Normalny") << tr("Wysoki");
	m_priotytetEdit->addItems(priotytetName);

	QStringList vatName;
	vatName << tr("0 %") << tr("3 %") << tr("5 %") << tr("8 %") << tr("23 %");
	m_vatEdit->addItems(vatName);
	m_vatEdit->setEnabled(false);
	m_vatEnable->setChecked(false);

	// wczytanie ceny podstawowej
	QSettings settings;
	m_price->setValue(settings.value("squaredPrice").toDouble());

	// ułożenie danych na temat zamówienia
	QFormLayout* m_orderLayout = new QFormLayout(m_orderGroup);
	m_orderLayout->addRow(tr("Tytuł"), m_titleEdit);
	m_orderLayout->addRow(tr("Data przyjęcia"), m_addDateEdit);
	m_orderLayout->addRow(tr("Termin realizacji"), m_endDateEdit);
	m_orderLayout->addRow(tr("Priotytet"), m_priotytetEdit);
	m_orderLayout->addRow(m_vatEnable, m_vatEdit);
	m_orderLayout->addRow(tr("Rabat [%]"), m_discountEdit);
	m_orderLayout->addRow(tr("Cena"), m_price);
	m_orderLayout->addRow(tr("Zrealizowane"), m_done);

	QVBoxLayout* contrahentLayout = new QVBoxLayout(m_contrahentGroup);
	contrahentLayout->addWidget(m_contrahentWidget);

	QVBoxLayout* noteLayout = new QVBoxLayout(m_notesGroup);
	noteLayout->addWidget(m_notesEdit);

	// ustawienie głównych kontenerów w oknie
	QHBoxLayout* topLayout = new QHBoxLayout;
	topLayout->addWidget(m_contrahentGroup);
	topLayout->addWidget(m_orderGroup);

	QVBoxLayout* mainLayout = new QVBoxLayout(widget);
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(m_notesGroup);

	return widget;
}

//-----------------------------------------------
// tworzy widgety dla zakładki zarządzania 
// elementami
QWidget* AddOrderDialog::elementsWidget()
{
	QWidget* widget = new QWidget;
	m_addButton		= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), widget);
	m_removeButton	= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), widget);
	m_duplicateButton = new QPushButton(tr("Powiel"), widget);
	m_sumLabel		= new QLabel(tr("<b>RAZEM 0 m<sup>3</sup></b>"), widget);
	m_planedLabel	= new QLabel(tr("<b>Strugane 0 m<sup>3</sup></b>"), widget);
	m_priceLabel	= new QLabel(tr("<b>Do zapłaty 0 zł</b>"), widget);
	m_itemsView		= new QTreeView(widget);
	m_sizeFilterGroup = new QGroupBox(tr("Filtr przekroju"), widget);
	m_widthValue = new QDoubleSpinBox(widget);
	m_heightValue = new QDoubleSpinBox(widget);
	m_rangeFilterButton = new QPushButton(tr("Filtruj"), widget);
	m_lengthFilterGroup = new QGroupBox(tr("Filter długości"), widget);
	m_minLength = new QDoubleSpinBox(widget);
	m_maxLength = new QDoubleSpinBox(widget);
	m_filterLengthButton = new QPushButton(tr("Filtruj"), widget);

	// przypisujemy modele do widoku
	m_model = new SquaredModel(this);

	m_elementsProxyModel = new ElementsFilterModel(this);
	m_elementsProxyModel->setSourceModel(m_model);
	m_itemsView->setItemDelegate(new SquaredDelegate(this));
	m_itemsView->setModel(m_elementsProxyModel);

	m_addDateEdit->setCalendarPopup(true);	// edycja jako lista rozwijada
	m_endDateEdit->setCalendarPopup(true);

	// zmieniamy politykę rozciągania się widgetów wyświetlających koszty
	m_sumLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_planedLabel->setSizePolicy(m_sumLabel->sizePolicy());
	m_priceLabel->setSizePolicy(m_sumLabel->sizePolicy());

	// wartości początkwe dla filtra wielkości
	m_sizeFilterGroup->setCheckable(true);
	m_sizeFilterGroup->setChecked(false);	// na początku filtr nie aktywny

	// wartości początkowe dla filtra długości elementów
	m_lengthFilterGroup->setCheckable(true);
	m_lengthFilterGroup->setChecked(false);

	// menu dla przycisku dodawania
	QMenu* addButtonMenu = new QMenu(this);
	addButtonMenu->addAction(tr("Dodaj"), this, SLOT(addSingleItem()), QKeySequence("Ins"));
	addButtonMenu->addAction(tr("Dodaj grupe elementów"), this, SLOT(addGroupItems()), QKeySequence("Ctrl+Ins"));
	m_addButton->setMenu(addButtonMenu);

	// przycisk operacje (tworzenie + menu)
	QPushButton* optionsButton = new QPushButton(tr("Operacje"), widget);
	QMenu* optionsButtonMenu = new QMenu(this);
	optionsButtonMenu->addAction(QIcon("icons/selectAll.png"), tr("Zaznacz wszystko"), m_model, SLOT(selectAll()));
	optionsButtonMenu->addAction(tr("Odznacz wszystko"), m_model, SLOT(unselectAll()));
	optionsButtonMenu->addSeparator();
	optionsButtonMenu->addAction(QIcon("icons/import.png"), tr("Import"), this, SLOT(importItemsFromFile()));
	optionsButtonMenu->addAction(QIcon("icons/export.png"), tr("Eksport"), this, SLOT(exportItemsToFile()));
	optionsButton->setMenu(optionsButtonMenu);	// dodajemy menu do przycisku

	QHBoxLayout* sizeLayout = new QHBoxLayout;
	sizeLayout->addWidget(new QLabel(tr("Szer"), widget));
	sizeLayout->addWidget(m_widthValue);
	sizeLayout->addWidget(new QLabel(tr("Wys"), widget));
	sizeLayout->addWidget(m_heightValue);

	QHBoxLayout* lengthLayout = new QHBoxLayout;
	lengthLayout->addWidget(new QLabel(tr("min"), widget));
	lengthLayout->addWidget(m_minLength);
	lengthLayout->addWidget(new QLabel(tr("max"), widget));
	lengthLayout->addWidget(m_maxLength);

	QHBoxLayout* rangeFilterButtonLayout = new QHBoxLayout;
	rangeFilterButtonLayout->addStretch(1);
	rangeFilterButtonLayout->addWidget(m_rangeFilterButton);

	QHBoxLayout* lengthFilterButtonLayout = new QHBoxLayout;
	lengthFilterButtonLayout->addStretch(1);
	lengthFilterButtonLayout->addWidget(m_filterLengthButton);

	QVBoxLayout* mainFilterLayout = new QVBoxLayout(m_sizeFilterGroup);
	mainFilterLayout->addLayout(sizeLayout);
	mainFilterLayout->addLayout(rangeFilterButtonLayout);

	QVBoxLayout* mainLengthFilterLayout = new QVBoxLayout(m_lengthFilterGroup);
	mainLengthFilterLayout->addLayout(lengthLayout);
	mainLengthFilterLayout->addLayout(lengthFilterButtonLayout);

	QHBoxLayout* labelLayout = new QHBoxLayout;
	labelLayout->addStretch(6);
	labelLayout->addWidget(m_planedLabel, Qt::AlignRight);
	labelLayout->addStretch(1);
	labelLayout->addWidget(m_sumLabel, Qt::AlignRight);
	labelLayout->addStretch(1);
	labelLayout->addWidget(m_priceLabel, Qt::AlignRight);

	// grupa zarządzania listą elementów
	QHBoxLayout* itemButtons = new QHBoxLayout;
	itemButtons->addWidget(m_addButton);
	itemButtons->addWidget(m_removeButton);
	itemButtons->addStretch(1);
	itemButtons->addWidget(optionsButton);
	itemButtons->addWidget(m_duplicateButton);
	itemButtons->addStretch(3);

	QHBoxLayout* filtersLayout = new QHBoxLayout;
	filtersLayout->addWidget(m_sizeFilterGroup);
	filtersLayout->addWidget(m_lengthFilterGroup);
	filtersLayout->addStretch(1);

	QVBoxLayout* mainLayout = new QVBoxLayout(widget);
	mainLayout->addLayout(filtersLayout);
	mainLayout->addWidget(m_itemsView);
	mainLayout->addLayout(labelLayout);
	mainLayout->addLayout(itemButtons);

	return widget;
}

//-----------------------------------------------
//widgety artykułów dodanych do zamóweinia
QWidget* AddOrderDialog::articleWidget()
{
	QWidget* widget = new QWidget(this);
	m_articleView	= new QTreeView(widget);
	m_addArticle	= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), widget);
	m_removeArticle	= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), widget);
	m_articlePriceLabel = new QLabel(tr("<b>Do zapłaty: 0zł</b>"), widget);

	m_articlePriceLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	// przypisanie modelu do widoku
	m_articleModel = new ArticleModel(this);
	m_articleView->setModel(m_articleModel);
	m_articleView->setItemDelegate(new ArticleDelegate(this));

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_addArticle);
	buttonLayout->addWidget(m_removeArticle);
	buttonLayout->addStretch(1);

	QHBoxLayout* labelLayout = new QHBoxLayout;
	labelLayout->addStretch(1);
	labelLayout->addWidget(m_articlePriceLabel);

	QVBoxLayout* mainLayout = new QVBoxLayout(widget);
	mainLayout->addWidget(m_articleView);
	mainLayout->addLayout(labelLayout);
	mainLayout->addLayout(buttonLayout);

	return widget;
}

//-----------------------------------------------
// aktualizuje stan wszystkich widgetów
// w zależności od potrzeb modelu
void AddOrderDialog::updateAllWidgets()
{
	m_sumLabel->setText(QString("<b>RAZEM %1 m<sup>3</sup></b>").arg(m_model->sumOfMetric()));
	m_planedLabel->setText(QString("<b>Strugane %1 m<sup>3</sup></b>").arg(m_model->planedOfMetric()));
	m_priceLabel->setText(QString("<b>Do zapłaty %1 zł</b>").arg(m_model->price()));
	m_articlePriceLabel->setText(tr("<b>Do zapłaty: %1zł</b>").arg(m_articleModel->priceToPay()));
	m_toPayLabel->setText(tr("<h3>Razem do zapłaty: %1 zł").arg(calculateMainPrice()));
}

//-----------------------------------------------
// zapisuje wprowadzone elementy do pliku XML
void AddOrderDialog::exportItemsToFile()
{
	const QString fileName = QFileDialog::getSaveFileName(this, tr("Eksport listy do pliku XML"),
		QDir::currentPath(), "*.xml\0*.*\0");

	if (!fileName.isEmpty())
		m_model->exportToXML(fileName);
}

//-----------------------------------------------
// importuje listę elementów z pliku XML
void AddOrderDialog::importItemsFromFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Import elementów z pliku XML"),
		QDir::currentPath(), "*.xml\0*.*\0");

	if (!fileName.isEmpty())
		m_model->importFromXML(fileName);
}

//-----------------------------------------------
// filtruje wiersze według określonych norm
void AddOrderDialog::filterByRange()
{/*
	if (m_widthColumn->isChecked())
		m_elementsProxyModel->setFilterColumn(0);
	else if (m_heightColumn->isChecked())
		m_elementsProxyModel->setFilterColumn(1);
	else if (m_lengthColumn->isChecked())
		m_elementsProxyModel->setFilterColumn(2);

	m_elementsProxyModel->setMinimumValue(m_minValue->value());
	m_elementsProxyModel->setMaximumValue(m_maxValue->value());*/

	m_elementsProxyModel->setWidth(m_widthValue->value());
	m_elementsProxyModel->setHeight(m_heightValue->value());
}

//-----------------------------------------------
// filtruje wiersze według długości elementów
void AddOrderDialog::filterByLength()
{
	m_elementsProxyModel->setMinLength(m_minLength->value());
	m_elementsProxyModel->setMaxLength(m_maxLength->value());
}

//-----------------------------------------------
// powiela zaznaczony rekord
void AddOrderDialog::duplicateSelectedItem()
{
	QItemSelectionModel* selectionModel = m_itemsView->selectionModel();
	QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

	m_model->addItem(selectedIndexes.at(0).data().toDouble(), selectedIndexes.at(1).data().toDouble(),
		selectedIndexes.at(2).data().toDouble(), selectedIndexes.at(3).data().toDouble(),
		selectedIndexes.at(4).data(Qt::EditRole).toBool());
}

//-----------------------------------------------
// aktywuje lub dezaktywuje wybór vatu
void AddOrderDialog::vatEnabled(int state)
{
	if (state)
	{
		m_vatEdit->setEnabled(true);
		m_model->setVatEnable(true);
	}
	else
	{
		m_vatEdit->setEnabled(false);
		m_model->setVatEnable(false);
	}
}

//-----------------------------------------------
// ustawia wartość VAT-u wybranego z listy
void AddOrderDialog::activatedVatValue(const QString & text)
{
	QString value = text;
	m_model->setVatValue(value.remove(" %").toInt());
}

//-----------------------------------------------
// dodaje nowy artykuł do zamówienia
void AddOrderDialog::addNewArticle()
{
	ArticlesManager* manager = new ArticlesManager(this, true);
	connect(manager, SIGNAL(articleSelected(int)), this, SLOT(addNewArticle(int)));
	manager->exec();
}

//-----------------------------------------------
// dodaje nowy artykuł do zamówienia
void AddOrderDialog::addNewArticle(int id)
{
	double count = QInputDialog::getDouble(this, tr("Dodawanie artykułu do zamówienia"),
		tr("Ilość"), 1);

	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid() && db.isOpen())
	{
		QSqlQuery query;
		query.prepare("SELECT idMagazynu, nazwa, cenaNetto, cenaBrutto, "
			"vat, jm, cenaDetaliczna, marza FROM artykuly WHERE idArtykulu = ?");

		query.bindValue(0, id);

		if (query.exec() && query.first())
		{
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

//-----------------------------------------------
// wysłanie kosztorysu zamówienia przez mail
void AddOrderDialog::sendByMailButton()
{
	if (m_contrahentWidget->mail().isEmpty())
	{
		QMessageBox::warning(this, tr("Ostrzeżenie"),
			tr("Wybrany kontrahet nie ma przypisanego adresu e-mail"));
	}
	else
	{
		QSettings settings;
		SmtpClient smtp(settings.value("mail/host").toString(), 
			settings.value("mail/port").toInt(), 
			settings.value("mail/sslConnection").toBool() ? SmtpClient::SslConnection : SmtpClient::TcpConnection);
		
		smtp.setUser(settings.value("mail/userName").toString());
		smtp.setPassword(settings.value("mail/password").toString());

		// wiadomość do wysłania
		MimeMessage message;

		EmailAddress sender(settings.value("mail/userName").toString(), settings.value("mail/sender").toString());
		message.setSender(&sender);

		EmailAddress to(m_contrahentWidget->mail(), "Kontrahent");
		message.addRecipient(&to);

		// nadanie tematu wiadomości
		message.setSubject(tr("Kosztorys zamówienia: %1").arg(m_titleEdit->text().toUpper()));

		// tworzenie właściwej treści wiadomości
		QString html;
		QTextStream out(&html);

		out << "<div style=\"float: left; width: 70%;\">"
			<< "<strong>" << settings.value("app/companyName").toString() << "</strong><br/>"
			<< settings.value("app/ownName").toString() << " " << settings.value("app/ownSurname").toString() << "<br/>"
			<< settings.value("app/city").toString() << " ";

		if (!settings.value("app/street").toString().isEmpty())
			out << "ul. " << settings.value("app/street").toString() << " ";

		out << settings.value("app/houseNumber").toString() << "<br/>";

		if (!settings.value("app/zipCode").toString().isEmpty() &&
			!settings.value("app/zipName").toString().isEmpty())
		{
			out << settings.value("app/zipCode").toString() << " "
				<< settings.value("app/zipName").toString() << "<br/>";
		}

		if (!settings.value("app/nip").toString().isEmpty())
			out << "NIP: " << settings.value("app/nip").toString() << "<br/>";

		if (!settings.value("app/regon").toString().isEmpty())
			out << "REGON: " << settings.value("app/regon").toString() << "<br/>";

		if (!settings.value("app/phoneNumber").toString().isEmpty())
			out << "Tel.: " << settings.value("app/phoneNumber").toString() << "<br/>";

		if (!settings.value("app/website").toString().isEmpty())
			out << settings.value("app/website").toString() << "<br/>";
		out	<< "</div>";

		out << "<div style=\"float: right; width: 30%; text-align: right;\">"
			<< settings.value("app/city").toString() << " dn. " << QDate::currentDate().toString("dd.MM-.yyy")
			<< "</div>";

		out << "<div style=\"width: 40%; float: right; text-align: left;\">"
			<< "<h3>Odbiorca:</h3>"
			<< m_contrahentWidget->contrahentAddress()
			<< "</div>";

		out << "<h2 style=\"width: 100%; margin: 12px auto; float: left; text-align: center;\">" << tr("Kosztorys zamówienia") << "<br/>" << m_titleEdit->text().toUpper() << "</h2>";

		out << "<div style=\"text-align: center; margin: 0 auto;\">";
		out << m_model->itemsToHtml();

		if (m_articleModel->listOfItems().size() > 0)
		{
			out << "<br/><h2>" << tr("Artykuły") << "</h2<br/>";
			out << m_articleModel->itemsToHtml();
		}

		// wyliczenie łącznej sumy do zapłaty
		double netto = calculateMainPrice();
		double vat = 100 + m_model->vatValue();
		double brutto = (netto*vat) / 100;

		out << "<p style=\"text-align: left;\">" << tr("Do zapłaty: ") << brutto << "</p><br/>";
		if (m_model->rabat())
			out << "<p style=\"text-align: left;\">Rabat: " << m_model->rabat() << "%</p><br/>";

		if (!m_notesEdit->document()->toPlainText().isEmpty())
		{
			out << "<h2 style=\"text-align: left;\">UWAGI:</h2>"
				<< "<span style=\"text-align: left;\">" << m_notesEdit->document()->toHtml() << "</span>";
		}
		out << "</div>";

		MimeHtml mimeHtml;
		mimeHtml.setHtml(html);
		message.addPart(&mimeHtml);

		bool bSuccess = true;
		if (!smtp.connectToHost())
		{
			QMessageBox::critical(this, tr("Błąd"),
				tr("Błąd połączenia z serwerem SMTP"));

			bSuccess = false;
		}
		
		if (!smtp.login())
		{
			QMessageBox::critical(this, tr("Błąd"),
				tr("Błąd logowania do serwera!"));

			bSuccess = false;
		}

		if (!smtp.sendMail(message))
		{
			QMessageBox::critical(this, tr("Błąd"),
				tr("Błąd wysyłania wiadomości e-mail"));

			bSuccess = false;
		}

		if (bSuccess)
		{
			QMessageBox::information(this, tr("Informacja"),
				tr("Wiadomość e-mail została wysłana do kontrahenta."));

			smtp.quit();
		}
	}
}

//-----------------------------------------------
// oblicza całkowity koszt do zapłaty
// uwzględniając rabat
double AddOrderDialog::calculateMainPrice()
{
	double cena = m_model->price() + m_articleModel->priceToPay();
	float r = 100 - rabat();
	float x = r/100;

	float toPay = cena*x;
	return toPay;
}

//-----------------------------------------------
// dodaje nowe wiersze do listy elementów
void AddOrderDialog::addSingleItem()
{
	AddSingleSquaredDialog* dialog = new AddSingleSquaredDialog(this);
	if (dialog->exec() == AddSingleSquaredDialog::Accepted)
	{
		m_model->addItem(dialog->width(), dialog->height(), dialog->length(), dialog->count(), dialog->isPlaned());

		QSettings settings;
		if (settings.value("squared/fastAdd").toBool())
			addSingleItem();
	}
}

//-----------------------------------------------
// tworzy dokument a następnie wysyła go
// do drukarki w celu wydrukowania kosztorysu
void AddOrderDialog::print()
{
	QString other;
	QTextStream out(&other);
	QSettings settings;

	if (m_articleModel->listOfItems().size() > 0)
	{
		out << "<center>";
		out << "<br><h3>" << tr("ARTYKUŁY") << "</h3>";
		out << m_articleModel->itemsToHtml();
		out << "</center>";
	}


	out << "<center>"
		<< "<h3>" << tr("PODSUMOWANIE") << "</h3>"
		<< "<table border=\"1\" style=\"font-size: 10px;\">";
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
		double netto = m_model->sumOfMetric()*price();
		double vat = 100 + m_model->vatValue();
		double brutto = (netto * vat) / 100;

		out << "<tr><td>" << tr("Elementy") << "</td>"
			<< "<td>" << m_model->sumOfMetric() << "</td>"
			<< "<td>" << price() << "</td>"
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

		netto = m_model->price();
		vat = 100 + m_model->vatValue();
		brutto = (netto * vat) / 100;

		out << "<tr><td colspan=\"3\"><b>" << tr("RAZEM") << "</b></td>"
			<< "<td>" << netto << "</td>"
			<< "<td>" << m_model->vatValue() << "%</td>"
			<< "<td>" << brutto + m_articleModel->priceToPay() << "</td></tr>";
	}
	else
	{
		out << "<tr><td>" << tr("Elementy") << "</td>"
			<< "<td>" << m_model->sumOfMetric() << "m<sup>3</sup></td>"
			<< "<td>" << price() << tr("zł") << "</td>"
			<< "<td>" << m_model->sumOfMetric() * price() << "</td></tr>";

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

		out << "<tr><td colspan=\"3\"><b>" << tr("RAZEM") << "</b></td>"
			<< "<td><b>" << m_model->price() + m_articleModel->priceToPay() << "</b></td></tr>";
	}

	out << "</table></center><br>";

	double netto;
	double vat = 100 + m_model->vatValue();
	double brutto;

	if (m_model->vatEnabled())
	{
		//netto = m_model->price();
		netto = calculateMainPrice();
		brutto = (netto * vat) / 100;
		brutto += m_articleModel->priceToPay();
	}
	else
	{
		netto = calculateMainPrice();
		brutto = (netto * vat) / 100;
	}

	out << "<h3 style=\"margin: 0px; padding: 0px;\">" << tr("Do zapłaty: ") << brutto << tr("zł") << "</h3>";
	if (m_model->rabat())
		out << tr("Rabat: ") << m_model->rabat() << "%<br>";

	if (!m_notesEdit->document()->toPlainText().isEmpty())
	{
		out << "<br><br><br><b>UWAGI:</b>";
		out << "<p>" << m_notesEdit->document()->toPlainText() << "</p>";
	}

	ClientDocument* document = new ClientDocument(this);
	document->setDocumentTitle(tr("Kosztorys zamówienia<br>%1").arg(m_titleEdit->text().toUpper()));
	document->prepareToPrint(m_contrahentWidget->contrahentAddress(), m_model->itemsToHtml(), other);

	// drukujemy dokument
	QPrinter* printer = new QPrinter;
	QPrintDialog* printDialog = new QPrintDialog(printer, this);
	if (printDialog->exec() == QPrintDialog::Accepted)
		document->print(printer);
}

//-----------------------------------------------
// dodawanie grupowe elementów konstrukcyjnych
void AddOrderDialog::addGroupItems()
{
	AddGroupSquaredDialog* dialog = new AddGroupSquaredDialog(this);
	if (dialog->exec() == AddGroupSquaredDialog::Accepted)
	{
		// pobieramy listę elementów z dialogu
		QList<SquaredRecord> tmp = dialog->itemsOfList();

		// a następnie przeglądamy całą i dodajemy elementy do modelu
		for (int i = 0; i < tmp.size(); i++)
			m_model->addItem(tmp[i].width(), tmp[i].height(), tmp[i].length(), tmp[i].count(), tmp[i].isPlaned());
	}
}
