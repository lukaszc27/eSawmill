#include "addServiceDialog.h"
#include <qlayout.h>
#include <qformlayout.h>
#include <qmenu.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qtextstream.h>
#include <qsettings.h>
#include <qinputdialog.h>

#include "addGroupSquaredDialog.h"
#include "addSingleSquaredDialog.h"
#include "squaredDelegate.h"
#include "roundDelegate.h"
#include "clientDocument.h"
#include "addRoundDialog.h"
#include "articlesManager.h"
#include "articleDelegate.h"
#include "smtpclient.h"
#include "mimehtml.h"


AddServiceDialog::AddServiceDialog(QWidget* parent, const bool updateMode, const int serviceId)
	: QDialog(parent)
{
	setWindowTitle(tr("Dodawanie nowej usługi"));
	createWidgets();
	createShortcut();

	if (updateMode)
	{
		setWindowTitle(tr("Aktualizacja usługi"));

		// jeśli tryb aktualizacji to wczytujemy dane
		loadFromDatabase(serviceId);
		m_elementsModel->loadFromService(serviceId);
		m_roundModel->loadFromService(serviceId);
		m_articleModel->loadItemsFromService(serviceId);

		// blokujemy odpowiednie widgety
		m_contrahentGroup->setEnabled(false);
		m_addDate->setEnabled(false);

		if (m_done->isChecked())
		{
			// zamówienie zostało zrealizowane
			m_contrahentGroup->setEnabled(false);
			m_serviceGroup->setEnabled(false);
			m_notesGroup->setEnabled(false);
			m_roundModel->setReadOnly(true);	// blokada wprowadzania danych
			m_elementsModel->readOnly(true);
		}
	}
	else setNearDate();

	connect(m_acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_rejectButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_rabat, SIGNAL(valueChanged(double)), m_roundModel, SLOT(setRabat(double)));
	connect(m_rangeFilterGroup, SIGNAL(clicked(bool)), m_elementsFilterModel, SLOT(setEnable(bool)));
	connect(m_rangeFilterButton, SIGNAL(clicked()), this, SLOT(rangeFilterButtonClicked()));
	connect(m_duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateSelectedItem()));
	connect(m_duplicateRoundButton, SIGNAL(clicked()), this, SLOT(duplicateRoundSelectedItems()));
	connect(m_vatCheck, SIGNAL(stateChanged(int)), this, SLOT(vatCheckState(int)));
	connect(m_vatValue, SIGNAL(activated(QString)), this, SLOT(setVatValue(QString)));
	connect(m_addArticle, SIGNAL(clicked()), this, SLOT(addArticle()));
	connect(m_removeArticle, SIGNAL(clicked()), m_articleModel, SLOT(removeCheckedItem()));
	connect(m_articleFilterText, SIGNAL(textChanged(QString)), this, SLOT(filterArticles(QString)));
}

//-----------------------------------------
// wylicza całkowity koszt do zapłaty
// uwzględniając rabat
double AddServiceDialog::calculateFullPrice()
{
	double roundPrice = m_roundModel->priceOfMetric();
	double articlePrice = m_articleModel->priceToPay();

	float r = (100 - rabat()) / 100;
	double price = roundPrice + articlePrice;

	return r*price;
}

//-----------------------------------------
// przygotowuje oraz drukuje spis kłód
// jako rozliczenie
void AddServiceDialog::printRounds()
{
	ClientDocument* document = new ClientDocument(this);
	document->setDocumentTitle(tr("Kosztorys usługi<br>%1").arg(m_title->text()));

	QString other;
	QTextStream out(&other);

	if (m_articleModel->listOfItems().size() > 0)
	{
		out << "<br /><center><h3>ARTYKULY</h3></center>";
		out << m_articleModel->itemsToHtml();
	}

	double netto = calculateFullPrice();
	double dvat = 100 + m_roundModel->vat();
	double brutto = (netto*dvat) / 100;

	out << "<br />";
	out << tr("<h3 style=\"padding: 0px; margin: 0px;\">Do zapłaty: ");
	if (m_roundModel->isVatEnable())
		out << brutto;
	else out << netto;

	out << tr("zł") << "</h3><br>";

	if (!m_notesEdit->document()->toPlainText().isEmpty())
	{
		out << "<p><b>UWAGI:</b></p>";
		out << m_notesEdit->document()->toPlainText();
	}

	document->prepareToPrint(m_contrahentWidget->contrahentAddress(), m_roundModel->itemsToHtml(), other);

	QPrinter* printer = new QPrinter;
	QPrintDialog* dialog = new QPrintDialog(printer, this);
	if (dialog->exec() == QPrintDialog::Accepted)
		document->print(printer);
}

//-----------------------------------------
// zwraca wybrany VAT
int AddServiceDialog::vat() const
{
	QString text = m_vatValue->currentText();
	text.remove(" %");
	m_roundModel->setVat(text.toInt());
	return text.toInt();
}

//-----------------------------------------
// drukuje spis elementów do wycięcia z
// dostarczonego drewna
void AddServiceDialog::printSquards()
{
	QString html;
	QTextStream out(&html);

	// tabelka przedstawiająca informacje o usłudze
	out << "<table>";
	out << tr("<tr><td>Tytuł</td><td>") << m_title->text() << "</td></tr>";
	out << tr("<tr><td>Zleceniodawca</td><td>") << m_contrahentWidget->contrahentAddress() << "</td></tr>";
	out << tr("<tr><td>Data przyjęcia</td><td>") << m_addDate->date().toString("dd.MM.yyyy") << "</td></tr>";
	out << tr("<tr><td>Termin realizacji</td><td>") << m_endDate->date().toString("dd.MM.yyyy") << "</td></tr>";
	out << tr("<tr><td>Uwagi</td><td>") << m_notesEdit->document()->toPlainText() << "</td></tr>";
	out << "</table>";
	out << "<br/>";
	
	// table wyświetlająca spis elementów do wycięcia
	out << "<table border=\"1\">";
	out << tr("<tr><th>Lp</th><th>Wymiar<br>[cm x cm]</th><th>Długość<br>[m]</th>"
			"<th width=\"80%\">Ilość<br>[szt]</th></tr>");
	
	QList<SquaredRecord> lists = m_elementsModel->listOfItems();
	for (int i = 0; i < lists.size(); i++)
	{
		if (lists[i].isPlaned())
			out << "<tr style=\"background-color: #e6e6e6;\">";
		else out << "<tr>";

		out	<< "<td>" << i + 1 << "</td>"
			<< "<td>" << lists[i].width() << "x" << lists[i].height() << "</td>"
			<< "<td>" << lists[i].length() << "</td>"
			<< "<td>" << lists[i].count() << "</td></tr>";
	}
	out << "</table>";

	// przygotowujemy dokument do drukowania
	QTextDocument* document = new QTextDocument(this);
	document->setDefaultFont(QFont("Courier New"));
	document->setHtml(html);
	
	// wyświetlamy dialog i drukujemy
	QPrinter* printer = new QPrinter;
	QPrintDialog* printDialog = new QPrintDialog(printer, this);
	if (printDialog->exec() == QPrintDialog::Accepted)
		document->print(printer);
}

//-----------------------------------------
// wysyła kosztorys usługi przez mail
void AddServiceDialog::sendByMail()
{
	if (m_contrahentWidget->mail().isEmpty())
	{
		QMessageBox::warning(this, tr("Ostrzeżenie"),
			tr("Podany kontrahent nie posiada przypisanego adresu e-mail!"));

		return;
	}
	else
	{
		QSettings settings;
		SmtpClient smtp(settings.value("mail/host").toString(),
			settings.value("mail/port").toInt(),
			settings.value("mail/sslConnection").toBool() ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

		smtp.setUser(settings.value("mail/userName").toString());
		smtp.setPassword(settings.value("mail/password").toString());

		//----
		MimeMessage message;
		message.addTo(new EmailAddress(m_contrahentWidget->mail(), "Kontrahent"));
		message.setSender(new EmailAddress(settings.value("mail/userName").toString(), 
			settings.value("mail/sender").toString()));

		message.setSubject(tr("Kosztorys usługi: %1").arg(m_title->text().toUpper()));

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
		out << "</div>";

		out << "<div style=\"float: right; width: 30%; text-align: right;\">"
			<< settings.value("app/city").toString() << " dn. " << QDate::currentDate().toString("dd.MM-.yyy")
			<< "</div>";

		out << "<div style=\"width: 40%; float: right; text-align: left;\">"
			<< "<h3>Odbiorca:</h3>"
			<< m_contrahentWidget->contrahentAddress()
			<< "</div>";

		out << "<h2 style=\"width: 100%; margin: 12px auto; float: left; text-align: center;\">" << tr("Kosztorys usługi") << "<br/>" << m_title->text().toUpper() << "</h2>";

		out << "<div style=\"text-align: center; width: 100%; float: left;\">";
		out << m_roundModel->itemsToHtml();

		if (m_articleModel->listOfItems().size() > 0)
		{
			out << "<br/><h2>" << tr("Artykuły") << "</h2<br/>";
			out << m_articleModel->itemsToHtml();
		}

		// wyliczenie łącznej sumy do zapłaty
		double netto = calculateFullPrice();
		double vat = 100 + m_roundModel->vat();
		double brutto = (netto*vat) / 100;

		out << "<p style=\"text-align: left;\"><h2>" << tr("Do zapłaty: ") << brutto << "</h2></p><br/>";
		if (m_roundModel->rabat())
			out << "<p style=\"text-align: left;\">Rabat: " << m_roundModel->rabat() << "%</p><br/>";

		if (!m_notesEdit->document()->toPlainText().isEmpty())
		{
			out << "<h2 style=\"text-align: left;\">UWAGI:</h2>"
				<< "<span style=\"text-align: left;\">" << m_notesEdit->document()->toHtml() << "</span>";
		}
		out << "</div>";

		MimeHtml mimeHtml;
		mimeHtml.setHtml(html);

		message.addPart(&mimeHtml);
		//----
		bool bSuccess = true;
		if (!smtp.connectToHost())
		{
			QMessageBox::critical(this, tr("Bład"),
				tr("Błąd nawiązywania połączenia z serwerem SMTP!"));

			bSuccess = false;
		}
		else if (!smtp.login())
		{
			QMessageBox::critical(this, tr("Błąd"),
				tr("Błąd logowania do serwera SMTP!"));

			bSuccess = false;
		}
		else if (!smtp.sendMail(message))
		{
			QMessageBox::critical(this, tr("Błąd"),
				tr("Bład wysyłania wiadomości e-mail"));

			bSuccess = false;
		}

		if (bSuccess)
		{
			QMessageBox::information(this, tr("Informacja"),
				tr("Wiadomość została wysłana do kontrahenta."));

			smtp.quit();
		}
	}
}

//-----------------------------------------
// tworzy dodatkowe widgety używane
// w oknie dodawania usługi
void AddServiceDialog::createWidgets()
{
	m_tabWidget = new QTabWidget(this);
	m_tabWidget->addTab(mainWidget(), tr("Ogólne"));
	m_tabWidget->addTab(roundWidget(), tr("Kłody"));
	m_tabWidget->addTab(elementWidget(), tr("Elementy"));
	m_tabWidget->addTab(articleWidget(), tr("Artykuły"));

	m_acceptButton		= new QPushButton(QIcon("icons/ok.png"), tr("OK"), this);
	m_rejectButton		= new QPushButton(QIcon("icons/cancel.png"), tr("Anuluj"), this);
	m_printRound		= new QPushButton(QIcon("icons/print.png"), tr("Drukuj"), this);
	m_sendByMailButton	= new QPushButton(QIcon("icons/mail.png"), tr("Wyślij przez e-mail"), this);
	m_fullPriceLabel	= new QLabel(tr("<h3>Razem do zapłaty: 0 zł</h3>"), this);

	m_fullPriceLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	connect(m_printRound, SIGNAL(clicked()), this, SLOT(printRounds()));
	connect(m_sendByMailButton, SIGNAL(clicked()), this, SLOT(sendByMail()));

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch(2);
	buttonsLayout->addWidget(m_printRound);
	buttonsLayout->addWidget(m_sendByMailButton);
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(m_acceptButton);
	buttonsLayout->addWidget(m_rejectButton);

	QHBoxLayout* fullPriceLayout = new QHBoxLayout;
	fullPriceLayout->addStretch(1);
	fullPriceLayout->addWidget(m_fullPriceLabel);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(m_tabWidget);
	mainLayout->addLayout(fullPriceLayout);
	mainLayout->addSpacing(24);
	mainLayout->addLayout(buttonsLayout);
}

//-----------------------------------------
// przypisuje skróty klawiszowe
void AddServiceDialog::createShortcut()
{
	m_addRound->setShortcut(QKeySequence("Ins"));
	m_removeRound->setShortcut(QKeySequence("Delete"));
	m_printRound->setShortcut(QKeySequence("Ctrl+P"));

	m_printElement->setShortcut(QKeySequence("Ctrl+P"));
	m_removeElement->setShortcut(QKeySequence("Delete"));

	m_acceptButton->setShortcut(QKeySequence("F10"));
	m_rejectButton->setShortcut(QKeySequence("ESC"));
}

//-----------------------------------------
// pobiera najwcześniejszą dane z bazy
// oraz ustawia w odpowiednich widgetach
// wylicza również przybliżony termin realizacji
void AddServiceDialog::setNearDate()
{
	QSqlDatabase db = QSqlDatabase::database();
	QDate date = QDate::currentDate();
	if (db.isOpen() && db.isValid())
	{
		QSqlQuery query;
		if (query.exec("select max(termin) from uslugi"))
		{
			if (query.first())
				date = query.value(0).toDate();
		}
		else date = QDate::currentDate();

		m_addDate->setDate(QDate::currentDate());
		date = date.addDays(3);	// dodajemy 3 dni do terminu realizacji
		m_endDate->setDate(date);
	}
}

//-----------------------------------------
// wczytuje usługe z bazy danych
// oraz umieszcza dane w odpowiednich polach
bool AddServiceDialog::loadFromDatabase(int id)
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid() && db.isOpen())
	{
		QSqlQuery q;
		q.prepare("select tytul, dataDodania, termin, uwagi, zrealizowane, idKontrahenta, rabat, vat, priorytet "
			"from uslugiwidok where id = ?");
		q.bindValue(0, id);

		if (q.exec())
		{
			if (q.first())
			{
				m_title->setText(q.value(0).toString());
				m_addDate->setDate(q.value(1).toDate());
				m_endDate->setDate(q.value(2).toDate());
				m_notesEdit->document()->setPlainText(q.value(3).toString());
				m_done->setChecked(q.value(4).toBool());
				m_contrahentWidget->loadContrahent(q.value(5).toInt());
				m_rabat->setValue(q.value(6).toDouble());
				m_priorytet->setCurrentIndex(q.value(8).toInt());

				if (q.value(7).toInt() > 0)
				{
					m_vatCheck->setChecked(true);
					QString text = tr("%1 %").arg(q.value(7).toInt());
					m_vatValue->setCurrentText(text);
					m_vatValue->setEnabled(true);

					m_roundModel->setVatEnabled(true);
					m_roundModel->setVat(q.value(7).toInt());
				}

				// ustawienie wielkości rabatu w modelach
				m_roundModel->setRabat(q.value(6).toDouble());
				m_elementsModel->setRabat(q.value(6).toDouble());
			}
			return true;
		}
		else
		{
			QMessageBox::critical(0, tr("Błąd"), q.lastError().text());
			return false;
		}
	}
	else
	{
		QMessageBox::critical(0, tr("Błąd"), db.lastError().text());
		return false;
	}
	return false;
}

//-----------------------------------------
// tworzy widgety dla głównej zakładki
// oraz zwraca otworzony widget
QWidget* AddServiceDialog::mainWidget()
{
	QWidget* widget		= new QWidget;
	m_contrahentGroup	= new QGroupBox(tr("Kontrahent"), widget);
	m_serviceGroup		= new QGroupBox(tr("Usługi"), widget);
	m_notesGroup		= new QGroupBox(tr("Uwagi"), widget);

	m_title		= new QLineEdit(widget);
	m_addDate	= new QDateEdit(widget);
	m_endDate	= new QDateEdit(widget);
	m_rabat		= new QDoubleSpinBox(widget);
	m_done		= new QCheckBox(widget);
	m_notesEdit = new QTextEdit(widget);
	m_vatCheck	= new QCheckBox(tr("VAT"), widget);
	m_vatValue	= new QComboBox(this);
	m_priorytet = new QComboBox(this);

	m_vatCheck->setChecked(false);
	m_vatValue->setEnabled(false);

	QStringList vatName;
	vatName << tr("0 %") << tr("3 %") << tr("5 %") << tr("8 %") << tr("23 %");
	m_vatValue->addItems(vatName);

	QStringList priorytetName;
	priorytetName << tr("Niski") << tr("Normalny") << tr("Wysoki");
	m_priorytet->addItems(priorytetName);

	m_contrahentWidget = new ContrahentWidget(this);

	m_addDate->setCalendarPopup(true);
	m_endDate->setCalendarPopup(this);

	QFormLayout* serviceLayout = new QFormLayout(m_serviceGroup);
	serviceLayout->addRow(tr("Tytuł"), m_title);
	serviceLayout->addRow(tr("Data dodania"), m_addDate);
	serviceLayout->addRow(tr("Termin"), m_endDate);
	serviceLayout->addRow(tr("Priorytet"), m_priorytet);
	serviceLayout->addRow(m_vatCheck, m_vatValue);
	serviceLayout->addRow(tr("Rabat [%]"), m_rabat);
	serviceLayout->addRow(tr("Zrealizowane"), m_done);

	QVBoxLayout* contrahentLayout = new QVBoxLayout(m_contrahentGroup);
	contrahentLayout->addWidget(m_contrahentWidget);

	QVBoxLayout* notesLayout = new QVBoxLayout(m_notesGroup);
	notesLayout->addWidget(m_notesEdit);

	// roztawienie grup na oknie
	QHBoxLayout* topLayout = new QHBoxLayout;
	topLayout->addWidget(m_contrahentGroup);
	topLayout->addWidget(m_serviceGroup);

	QVBoxLayout* mainLayout = new QVBoxLayout(widget);
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(m_notesGroup);

	return widget;
}

//-----------------------------------------
// tworzy widgety dla zakładki kłody
QWidget* AddServiceDialog::roundWidget()
{
	QWidget* widget = new QWidget;

	m_roundView		= new QTreeView(this);
	m_addRound		= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), widget);
	m_removeRound	= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), widget);
	m_optionRound	= new QPushButton(tr("Operacje"), widget);
	m_sumLabel		= new QLabel(tr("<b>RAZEM: 0 m<sup>3</sup></b>"), widget);
	m_priceLabel = new QLabel(tr("<b>Do zapłaty: 0 zł</b>"));
	m_duplicateRoundButton = new QPushButton(tr("Powiel"), widget);

	// ograniczamy możliwość rozciągania widgetów
	m_sumLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_priceLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	m_roundModel = new RoundModel(this);
	m_roundView->setModel(m_roundModel);
	m_roundView->setItemDelegate(new RoundDelegate(this));

	// łączymy sygnał aktualizujący dane w oknie
	connect(m_roundModel, SIGNAL(modelChanged()), this, SLOT(updateAllWidgets()));

	// menu dla przycisku operacje
	QMenu* optionMenu = new QMenu(m_optionRound);
	optionMenu->addAction(QIcon("icons/selectAll.png"), tr("Zaznacz wszystko"), 
		m_roundModel, SLOT(selectAll()), QKeySequence("Ctrl+A"));
	optionMenu->addAction(tr("Odznacz wszystko"), m_roundModel, SLOT(unselectAll()));
	optionMenu->addSeparator();
	optionMenu->addAction(QIcon("icons/import.png"), tr("Import"), this, SLOT(importRoundsFromFile()));
	optionMenu->addAction(QIcon("icons/export.png"), tr("Eksport"), this, SLOT(exportRoundsToFile()));
	m_optionRound->setMenu(optionMenu);

	QHBoxLayout* labelsLayout = new QHBoxLayout;
	labelsLayout->addStretch(6);
	labelsLayout->addWidget(m_sumLabel, 0, Qt::AlignRight);
	labelsLayout->addStretch(1);
	labelsLayout->addWidget(m_priceLabel, 0, Qt::AlignRight);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_addRound);
	buttonsLayout->addWidget(m_removeRound);
	buttonsLayout->addStretch(2);
	buttonsLayout->addWidget(m_optionRound);
	buttonsLayout->addWidget(m_duplicateRoundButton);
	buttonsLayout->addStretch(6);

	QVBoxLayout* mainLayout = new QVBoxLayout(widget);
	mainLayout->addWidget(m_roundView);
	mainLayout->addLayout(labelsLayout);
	mainLayout->addLayout(buttonsLayout);

	// tworzenie połączeń sygnał -> slot
	connect(m_addRound, SIGNAL(clicked()), this, SLOT(addRound()));
	connect(m_removeRound, SIGNAL(clicked()), m_roundModel, SLOT(removeCheckedItems()));

	return widget;
}

//-----------------------------------------
// tworzy widgety dla zakładki elementy
QWidget * AddServiceDialog::elementWidget()
{
	QWidget* widget = new QWidget;

	m_elementsView	= new QTreeView(widget);
	m_addElement	= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), widget);
	m_removeElement = new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), widget);
	m_optionElement = new QPushButton(tr("Operacje"), this);
	m_printElement	= new QPushButton(QIcon("icons/print.png"), tr("Drukuj"), widget);
	m_duplicateButton = new QPushButton(tr("Powiel"), widget);
	m_rangeFilterGroup	= new QGroupBox(tr("Filter"), widget);
	m_rangeFilterButton = new QPushButton(tr("Filtruj"), widget);
	m_widthColumn		= new QRadioButton(tr("Szerokość"), widget);
	m_heightColumn		= new QRadioButton(tr("Wysokość"), widget);
	m_lengthColumn		= new QRadioButton(tr("Długość"), widget);
	m_minValue			= new QDoubleSpinBox(widget);
	m_maxValue			= new QDoubleSpinBox(widget);

	m_rangeFilterGroup->setCheckable(true);
	m_rangeFilterGroup->setChecked(false);

	// tworzymy oraz ustawaimy model dla widoku
	m_elementsModel = new SquaredModel(this);
	m_elementsFilterModel = new ElementsFilterModel(this);
	m_elementsFilterModel->setSourceModel(m_elementsModel);
	m_elementsView->setModel(m_elementsFilterModel);
	m_elementsView->setItemDelegate(new SquaredDelegate(this));

	// menu "operacje"
	QMenu* optionMenu = new QMenu(m_optionElement);
	optionMenu->addAction(QIcon("icons/selectAll.png"), tr("Zaznacz wszystko"), 
		m_elementsModel, SLOT(selectAll()), QKeySequence("Ctrl+A"));
	optionMenu->addAction(tr("Odznacz wszystko"), m_elementsModel, SLOT(unselectAll()));
	optionMenu->addSeparator();
	optionMenu->addAction(QIcon("icons/import.png"), tr("Import"), this, SLOT(importFromFile()));
	optionMenu->addAction(QIcon("icons/export.png"), tr("Eksport"), this, SLOT(exportToFile()));
	m_optionElement->setMenu(optionMenu);

	// menu dla przycisku "Dodaj"
	QMenu* addMenu = new QMenu(m_addElement);
	addMenu->addAction(tr("Dodaj"), this, SLOT(addSingleElement()), QKeySequence("Ins"));
	addMenu->addAction(tr("Dodaj grupe elementów"), this, SLOT(addGroupElement()), QKeySequence("Ctrl+Ins"));
	m_addElement->setMenu(addMenu);

	QHBoxLayout* rangeColumnFilterlayout = new QHBoxLayout;
	rangeColumnFilterlayout->addWidget(m_widthColumn);
	rangeColumnFilterlayout->addWidget(m_heightColumn);
	rangeColumnFilterlayout->addWidget(m_lengthColumn);

	QHBoxLayout* rangeFilterEditLayout = new QHBoxLayout;
	rangeFilterEditLayout->addWidget(new QLabel(tr("Od"), widget));
	rangeFilterEditLayout->addWidget(m_minValue);
	rangeFilterEditLayout->addWidget(new QLabel(tr("Do"), widget));
	rangeFilterEditLayout->addWidget(m_maxValue);

	QHBoxLayout* rangeFilterButtonLayouts = new QHBoxLayout;
	rangeFilterButtonLayouts->addStretch(1);
	rangeFilterButtonLayouts->addWidget(m_rangeFilterButton);

	QVBoxLayout* rangeFilterMainLayout = new QVBoxLayout(m_rangeFilterGroup);
	rangeFilterMainLayout->addLayout(rangeColumnFilterlayout);
	rangeFilterMainLayout->addLayout(rangeFilterEditLayout);
	rangeFilterMainLayout->addLayout(rangeFilterButtonLayouts);

	QHBoxLayout* filtersLayout = new QHBoxLayout;
	filtersLayout->addWidget(m_rangeFilterGroup);
	filtersLayout->addStretch(1);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_addElement);
	buttonsLayout->addWidget(m_removeElement);
	buttonsLayout->addStretch(2);
	buttonsLayout->addWidget(m_optionElement);
	buttonsLayout->addWidget(m_duplicateButton);
	buttonsLayout->addWidget(m_printElement);
	buttonsLayout->addStretch(6);

	QVBoxLayout* mainLayout = new QVBoxLayout(widget);
	mainLayout->addLayout(filtersLayout);
	mainLayout->addWidget(m_elementsView);
	mainLayout->addLayout(buttonsLayout);

	// tworzymy połączenia sygnałów ze slotami
	connect(m_removeElement, SIGNAL(clicked()), m_elementsModel, SLOT(removeCheckedItems()));
	connect(m_printElement, SIGNAL(clicked()), this, SLOT(printSquards()));

	return widget;
}

//-----------------------------------------
// tworzy zakładkę dla artykułów
QWidget* AddServiceDialog::articleWidget()
{
	QWidget* widget = new QWidget(this);

	m_articleView			= new QTreeView(widget);
	m_articleFilterColumn	= new QComboBox(widget);
	m_articleFilterText		= new QLineEdit(widget);
	m_addArticle			= new QPushButton(QIcon("icons/add.png"), tr("Dodaj"), widget);
	m_removeArticle			= new QPushButton(QIcon("icons/remove.png"), tr("Usuń"), widget);
	m_toPayLabel			= new QLabel(tr("<b>Do zapłaty: 0 zł</b>"), widget);

	m_toPayLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	m_articleModel = new ArticleModel(this);
	m_articleProxyModel = new QSortFilterProxyModel(this);
	m_articleProxyModel->setSourceModel(m_articleModel);
	m_articleView->setModel(m_articleProxyModel);
	m_articleView->setItemDelegate(new ArticleDelegate(this));
	m_articleFilterColumn->addItems(m_articleModel->headerList());

	connect(m_articleModel, SIGNAL(modelChanged()), this, SLOT(updateAllWidgets()));

	QHBoxLayout* filterLayout = new QHBoxLayout;
	filterLayout->addWidget(m_articleFilterColumn);
	filterLayout->addWidget(m_articleFilterText);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_addArticle);
	buttonsLayout->addWidget(m_removeArticle);
	buttonsLayout->addStretch(1);

	QHBoxLayout* labelsLayout = new QHBoxLayout;
	labelsLayout->addStretch(1);
	labelsLayout->addWidget(m_toPayLabel);

	QVBoxLayout* mainLayout = new QVBoxLayout(widget);
	mainLayout->addLayout(filterLayout);
	mainLayout->addWidget(m_articleView);
	mainLayout->addLayout(labelsLayout);
	mainLayout->addLayout(buttonsLayout);

	return widget;
}

//-----------------------------------------
// dodaje grupe elementów do listy
void AddServiceDialog::addGroupElement()
{
	AddGroupSquaredDialog* dialog = new AddGroupSquaredDialog(this);
	if (dialog->exec() == QDialog::Accepted)
	{
		QList<SquaredRecord> tmp = dialog->itemsOfList();
		for (int i = 0; i < tmp.size(); i++)
			m_elementsModel->addItem(tmp[i].width(), tmp[i].height(), tmp[i].length(),
				tmp[i].count(), tmp[i].isPlaned(), false);
	}
}

//-----------------------------------------
// dodaje nową kłode do listy
void AddServiceDialog::addRound()
{
	AddRoundDialog* dialog = new AddRoundDialog(this);
	if (dialog->exec() == AddRoundDialog::Accepted)
	{
		m_roundModel->addItem(dialog->diameter(), dialog->length(), dialog->isEaves());

		QSettings settings;
		if (settings.value("round/fastAdd").toBool())
			addRound();
	}
}

//-----------------------------------------
// wczytuje elementy zapisane w pliku XML
void AddServiceDialog::importFromFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Import elementów z pliku XML"),
		QDir::currentPath(), "*.xml\0*.*");

	if (!fileName.isEmpty())
		m_elementsModel->importFromXML(fileName);
}

//-----------------------------------------
// zapisuje elementy z listy do pliku
void AddServiceDialog::exportToFile()
{
	const QString fileName = QFileDialog::getSaveFileName(this, tr("Eksport elementów do pliku XML"),
		QDir::currentPath(), "*.xml\0*.*\0");

	if (!fileName.isEmpty())
		m_elementsModel->exportToXML(fileName);
}

//-----------------------------------------
// wczytuje listę kłód z pliku XML
void AddServiceDialog::importRoundsFromFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Import elementów z pliku XML"),
		QDir::currentPath(), "*.xml\0*.*\0");

	if (!fileName.isEmpty())
		m_roundModel->importFromXML(fileName);
}

//-----------------------------------------
// zapisuje kłody do pliku XML
void AddServiceDialog::exportRoundsToFile()
{
	const QString fileName = QFileDialog::getSaveFileName(this, tr("Eksport do pliku XML"),
		QDir::currentPath(), "*.xml\0*.*");

	if (!fileName.isEmpty())
		m_roundModel->exportToXML(fileName);
}

//-----------------------------------------
// aktualizuje stan wszystkich widgetów
// na podstawie modelu
void AddServiceDialog::updateAllWidgets()
{
	m_sumLabel->setText(tr("<b>RAZEM: %1 m<sup>3</sup></b>")
		.arg(m_roundModel->sumOfMetric() + m_roundModel->sumOfEavesMetric()));
	
	m_priceLabel->setText(tr("<b>Do zapłaty: %1 zł</b>")
		.arg(m_roundModel->priceOfMetric()));

	m_toPayLabel->setText(tr("<b>Do zapłaty: %1 zł</b>").arg(m_articleModel->priceToPay()));
	m_fullPriceLabel->setText(tr("<h3>Razem do zapłaty: %1</h3>").arg(calculateFullPrice()));
}

//-----------------------------------------
// filtruje wiersze według wskazanych kategorii
void AddServiceDialog::rangeFilterButtonClicked()
{
	if (m_widthColumn->isChecked())
		m_elementsFilterModel->setFilterColumn(0);
	else if (m_heightColumn->isChecked())
		m_elementsFilterModel->setFilterColumn(1);
	else if (m_lengthColumn->isChecked())
		m_elementsFilterModel->setFilterColumn(2);

	m_elementsFilterModel->setMinimumValue(m_minValue->value());
	m_elementsFilterModel->setMaximumValue(m_maxValue->value());
}

//-----------------------------------------
// powiela zanaczony element
void AddServiceDialog::duplicateSelectedItem()
{
	QItemSelectionModel* selectionModel = m_elementsView->selectionModel();
	QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

	m_elementsModel->addItem(selectedIndexes.at(0).data().toDouble(),
		selectedIndexes.at(1).data().toDouble(),
		selectedIndexes.at(2).data().toDouble(),
		selectedIndexes.at(3).data().toDouble(),
		selectedIndexes.at(4).data(Qt::EditRole).toBool());
}

//-----------------------------------------
// duplikuje kłody
void AddServiceDialog::duplicateRoundSelectedItems()
{
	QItemSelectionModel* selectionModel = m_roundView->selectionModel();
	QModelIndexList selectionIndexes = selectionModel->selectedIndexes();

	m_roundModel->addItem(selectionIndexes.at(0).data().toDouble(),
		selectionIndexes.at(1).data().toDouble(),
		selectionIndexes.at(2).data(Qt::EditRole).toBool());
}

//-----------------------------------------
// aktywuje lub dezaktywuje naliczanie VAT-u
void AddServiceDialog::vatCheckState(int state)
{
	if (state)
	{
		m_vatValue->setEnabled(true);
		m_roundModel->setVatEnabled(true);
	}
	else
	{
		m_vatValue->setEnabled(false);
		m_roundModel->setVatEnabled(false);
	}
}

//-----------------------------------------
// ustawia wartość vat-u wybraną z listy
// rozwijanej
void AddServiceDialog::setVatValue(const QString & value)
{
	QString text = value;
	text.remove(" %");
	m_roundModel->setVat(text.toInt());
}

//-----------------------------------------
// wywołuje menadżera artykułów w
// trybie wyboru
void AddServiceDialog::addArticle()
{
	ArticlesManager* manager = new ArticlesManager(this, true);
	connect(manager, SIGNAL(articleSelected(int)), this, SLOT(addArticle(int)));

	manager->exec();
}

//-----------------------------------------
// dodaje do listy wybrany artykuł
void AddServiceDialog::addArticle(int id)
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

//-----------------------------------------
// filtruje artykuły według zadanych kryteriów
void AddServiceDialog::filterArticles(const QString& text)
{
	if (text.isEmpty())
	{
		m_articleProxyModel->setFilterKeyColumn(0);
		m_articleProxyModel->setFilterRegExp("");
		m_articleProxyModel->invalidate();
	}
	else
	{
		QString pattern = tr("(%1)([A-Za-z0-9.]*)").arg(text.toUpper());
		m_articleProxyModel->setFilterRegExp(pattern);
		m_articleProxyModel->setFilterKeyColumn(m_articleFilterColumn->currentIndex());
	}
}

//-----------------------------------------
// dodaje pojedynczy element do listy
void AddServiceDialog::addSingleElement()
{
	AddSingleSquaredDialog* dialog = new AddSingleSquaredDialog(this);
	if (dialog->exec() == AddSingleSquaredDialog::Accepted)
	{
		m_elementsModel->addItem(dialog->width(), dialog->height(), dialog->length(),
			dialog->count(), dialog->isPlaned());

		QSettings settings;
		if (settings.value("squared/fastAdd").toBool())
			addSingleElement();
	}
}
