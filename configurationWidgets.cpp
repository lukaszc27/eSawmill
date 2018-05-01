#include "configurationWidgets.h"
#include <qgridlayout.h>
#include <qformlayout.h>
#include <qlayout.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include "smtpclient.h"


ConfigurationWidget::ConfigurationWidget(QWidget *parent)
	: QWidget(parent)
{
}

ConfigurationMainPage::ConfigurationMainPage(QWidget* parent)
	: ConfigurationWidget(parent)
{
	setWindowTitle(tr("Konfiguracja firmy"));
	createWidgets();
	initialize();

	connect(m_loadButton, SIGNAL(clicked()), this, SLOT(loadBannerFromFile()));
}

//-------------------------------------------------------
// zapisuje wprowadzoną konfiguracje
void ConfigurationMainPage::save()
{
	QSettings settings;
	settings.setValue("app/companyName", m_companyName->text().toUpper());
	settings.setValue("app/ownName", m_ownName->text().toUpper());
	settings.setValue("app/ownSurname", m_ownSurname->text().toUpper());
	settings.setValue("app/nip", m_nip->text().toUpper());
	settings.setValue("app/regon", m_regon->text().toUpper());
	settings.setValue("app/pesel", m_pesel->text().toUpper());
	settings.setValue("app/city", m_city->text().toUpper());
	settings.setValue("app/zipCode", m_zipCode->text().toUpper());
	settings.setValue("app/zipName", m_zipName->text().toUpper());
	settings.setValue("app/street", m_street->text().toUpper());
	settings.setValue("app/houseNumber", m_houseNumber->text().toUpper());
	settings.setValue("app/phoneNumber", m_phoneNumber->text().toUpper());
	settings.setValue("app/mail", m_mail->text().toUpper());
	settings.setValue("app/website", m_website->text().toUpper());
	settings.setValue("app/bank", m_bank->text().toUpper());
	settings.setValue("app/bankNumber", m_bankNumber->text().toUpper());

	// zapis stylu aplikacji
	settings.setValue("app/style", m_appStyle->currentIndex());
}

//-------------------------------------------------------
// tworzenie widgetów
void ConfigurationMainPage::createWidgets()
{
	m_titleLabel = new QLabel(tr("<h2>Głowna konfiguracja programu</h2>"), this);

	m_companyName	= new QLineEdit(this);
	m_ownName		= new QLineEdit(this);
	m_ownSurname	= new QLineEdit(this);
	m_nip			= new QLineEdit(this);
	m_regon			= new QLineEdit(this);
	m_pesel			= new QLineEdit(this);
	m_city			= new QLineEdit(this);
	m_zipCode		= new QLineEdit(this);
	m_zipName		= new QLineEdit(this);
	m_street		= new QLineEdit(this);
	m_houseNumber	= new QLineEdit(this);
	m_phoneNumber	= new QLineEdit(this);
	m_mail			= new QLineEdit(this);
	m_website		= new QLineEdit(this);
	m_bank			= new QLineEdit(this);
	m_bankNumber	= new QLineEdit(this);

	m_bannerGroup	= new QGroupBox(tr("Logo firmowe"), this);
	m_loadButton	= new QPushButton(tr("Wczytaj obraz z dysku"), this);
	m_banner		= new QLabel(this);
	m_appStyleGroup = new QGroupBox(tr("Styl graficzny aplikacji"), this);
	m_appStyle		= new QComboBox(this);

	QStringList appStyleItems;
	appStyleItems << tr("Normalny") << tr("Jasny") << tr("Ciemny");
	m_appStyle->addItems(appStyleItems);

	QHBoxLayout* titleLayout = new QHBoxLayout;
	titleLayout->addWidget(m_titleLabel);

	QHBoxLayout* bunnerButtonLayout = new QHBoxLayout;
	bunnerButtonLayout->addWidget(m_loadButton);
	bunnerButtonLayout->addStretch(1);

	QVBoxLayout* mainLogoLayout = new QVBoxLayout(m_bannerGroup);
	mainLogoLayout->addLayout(bunnerButtonLayout);
	mainLogoLayout->addWidget(m_banner);

	QHBoxLayout* bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(m_bannerGroup);
	bottomLayout->addWidget(m_appStyleGroup);

	QVBoxLayout* styleLayout = new QVBoxLayout(m_appStyleGroup);
	styleLayout->addWidget(m_appStyle);

	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->addWidget(new QLabel(tr("Nazwa firmy"), this), 0, 0);	mainLayout->addWidget(m_companyName, 0, 1);
	mainLayout->addWidget(new QLabel(tr("Imię"), this), 1, 0);			mainLayout->addWidget(m_ownName, 1, 1);
	mainLayout->addWidget(new QLabel(tr("Nazwisko"), this), 2, 0);		mainLayout->addWidget(m_ownSurname, 2, 1);
	mainLayout->addWidget(new QLabel(tr("NIP"), this), 3, 0);			mainLayout->addWidget(m_nip, 3, 1);
	mainLayout->addWidget(new QLabel(tr("REGON"), this), 4, 0);			mainLayout->addWidget(m_regon, 4, 1);
	mainLayout->addWidget(new QLabel(tr("PESEL"), this), 5, 0);			mainLayout->addWidget(m_pesel, 5, 1);
	mainLayout->addWidget(new QLabel(tr("Miejscowość"), this), 6, 0);	mainLayout->addWidget(m_city, 6, 1);
	mainLayout->addWidget(new QLabel(tr("Kod pocztowy"), this), 7, 0);	mainLayout->addWidget(m_zipCode, 7, 1);

	mainLayout->addWidget(new QLabel(tr("Poczta"), this), 0, 2);					mainLayout->addWidget(m_zipName, 0, 3);
	mainLayout->addWidget(new QLabel(tr("Ulica"), this), 1, 2);						mainLayout->addWidget(m_street, 1, 3);
	mainLayout->addWidget(new QLabel(tr("Numer domu/mieszkania"), this), 2, 2);		mainLayout->addWidget(m_houseNumber, 2, 3);
	mainLayout->addWidget(new QLabel(tr("Numer telefonu"), this), 3, 2);			mainLayout->addWidget(m_phoneNumber, 3, 3);
	mainLayout->addWidget(new QLabel(tr("E-Mail"), this), 4, 2);					mainLayout->addWidget(m_mail, 4, 3);
	mainLayout->addWidget(new QLabel(tr("Strona WWW"), this), 5, 2);				mainLayout->addWidget(m_website, 5, 3);
	mainLayout->addWidget(new QLabel(tr("Bank"), this), 6, 2);						mainLayout->addWidget(m_bank, 6, 3);
	mainLayout->addWidget(new QLabel(tr("Nr. rachunku bankowego"), this), 7, 2);	mainLayout->addWidget(m_bankNumber, 7, 3);

	QVBoxLayout* myLayout = new QVBoxLayout(this);
	myLayout->addLayout(titleLayout);
	myLayout->addStretch(1);
	myLayout->addLayout(mainLayout);
	myLayout->addStretch(1);
	myLayout->addLayout(bottomLayout);
}

//-------------------------------------------------------
// wczytuje zapisaną konfiguracje
void ConfigurationMainPage::initialize()
{
	QSettings settings;
	m_companyName->setText(settings.value("app/companyName").toString());
	m_ownName->setText(settings.value("app/ownName").toString());
	m_ownSurname->setText(settings.value("app/ownSurname").toString());
	m_nip->setText(settings.value("app/nip").toString());
	m_regon->setText(settings.value("app/regon").toString());
	m_pesel->setText(settings.value("app/pesel").toString());
	m_city->setText(settings.value("app/city").toString());
	m_zipCode->setText(settings.value("app/zipCode").toString());
	m_zipName->setText(settings.value("app/zipName").toString());
	m_street->setText(settings.value("app/street").toString());
	m_houseNumber->setText(settings.value("app/houseNumber").toString());
	m_phoneNumber->setText(settings.value("app/phoneNumber").toString());
	m_mail->setText(settings.value("app/mail").toString());
	m_website->setText(settings.value("app/website").toString());
	m_bank->setText(settings.value("app/bank").toString());
	m_bankNumber->setText(settings.value("app/bankNumber").toString());

	if (!settings.value("app/banner").toString().isEmpty())
	{
		QPixmap pixmap;
		if (pixmap.load(settings.value("app/banner").toString()))
			m_banner->setPixmap(pixmap);
	}

	// wczytanie aktualnie używanego stylu graficznego
	m_appStyle->setCurrentIndex(settings.value("app/style").toInt());
}

//-------------------------------------------------------
// wczytuje banner z pliku sprawdza czy wymiary odpowiadają
// wytycznym
void ConfigurationMainPage::loadBannerFromFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Wczytywanie obrazu"),
		QDir::currentPath(), "*.*");

	if (!fileName.isEmpty())
	{
		QPixmap pixmap;
		if (pixmap.load(fileName))
		{
			if (pixmap.width() <= 128 && pixmap.height() <= 64)
			{
				QSettings settings;
				settings.setValue("app/banner", fileName);	// zapisujemy adres do pliku w konfiguracji

				// wyświetlamy w podglądzie
				m_banner->setPixmap(pixmap);
			}
			else
			{
				QMessageBox::warning(this, tr("Ostrzeżenie"),
					tr("Wczytany obraz nie spełnia wytycznych co do wymiaru!\n"
						"Obraz powinien mieć wymiary nie większe niż 128x64 pikseli"));

				return;
			}
		}
		else
		{
			QMessageBox::critical(this, tr("Błąd"),
				tr("Podczas wczytywania pliku wystąpiły błędy"));

			return;
		}
	}
}

//=============================================================
ConfigurationOrderPage::ConfigurationOrderPage(QWidget* parent)
	: ConfigurationWidget(parent)
{
	setWindowTitle(tr("Zamówienia"));
	createWidgets();
	initialize();
}

//-------------------------------------------------------
// zapis konfiguracji
void ConfigurationOrderPage::save()
{
	QSettings settings;
	settings.setValue("squaredPrice", m_priceEdit->value());
	settings.setValue("squared/fastAdd", m_fastAdd->isChecked());
}

//-------------------------------------------------------
// tworzenie widgetów
void ConfigurationOrderPage::createWidgets()
{
	m_priceEdit = new QDoubleSpinBox(this);
	m_fastAdd	= new QCheckBox(this);

	m_priceEdit->setMaximum(10000);

	QLabel* titleLabel = new QLabel(tr("<h2>Konfiguracja zamówień</h2>"), this);

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(tr("Cena [zł/m<sup>3</sup>]"), m_priceEdit);
	formLayout->addRow(tr("Szybkie wprowadzanie elementów"), m_fastAdd);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(titleLabel);
	mainLayout->addStretch(1);
	mainLayout->addLayout(formLayout);
	mainLayout->addStretch(3);
}

//-------------------------------------------------------
// wczytanie konfiguracji
void ConfigurationOrderPage::initialize()
{
	QSettings settings;
	m_priceEdit->setValue(settings.value("squaredPrice").toDouble());
	m_fastAdd->setChecked(settings.value("squared/fastAdd").toBool());
}

//=============================================================
ConfigurationServicePage::ConfigurationServicePage(QWidget* parent)
	: ConfigurationWidget(parent)
{
	setWindowTitle(tr("Usługi"));
	createWidgets();
	initialize();
}

//-------------------------------------------------------
// zapis konfiguracji
void ConfigurationServicePage::save()
{
	QSettings settings;
	settings.setValue("roundPrice", m_roundPrice->value());
	settings.setValue("roundEavesPrice", m_eavesRoundPrice->value());
	settings.setValue("planedPrice", m_planedPrice->value());
	settings.setValue("round/fastAdd", m_fastAdd->isChecked());
}

//-------------------------------------------------------
// tworzenie widgetów
void ConfigurationServicePage::createWidgets()
{
	m_roundPrice		= new QDoubleSpinBox(this);
	m_eavesRoundPrice	= new QDoubleSpinBox(this);
	m_planedPrice		= new QDoubleSpinBox(this);
	m_fastAdd			= new QCheckBox(this);

	m_roundPrice->setMaximum(10000);
	m_eavesRoundPrice->setMaximum(10000);
	m_planedPrice->setMaximum(10000);

	QLabel* titleLabel = new QLabel(tr("<h2>Konfiguracja usług</h2>"), this);

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(tr("Cena zwykła [zł/m<sup>3</sup>]"), m_roundPrice);
	formLayout->addRow(tr("Cena kantowana [zł/m<sup>3</sup>]"), m_eavesRoundPrice);
	formLayout->addRow(tr("Cena usługi strugania [zł/m<sup>3</sup>]"), m_planedPrice);
	formLayout->addRow(tr("Szybkie wprowadzanie kłód"), m_fastAdd);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(titleLabel);
	mainLayout->addStretch(1);
	mainLayout->addLayout(formLayout);
	mainLayout->addStretch(2);
}

//-------------------------------------------------------
// wczytanie konfiguracji
void ConfigurationServicePage::initialize()
{
	QSettings settings;
	m_roundPrice->setValue(settings.value("roundPrice").toDouble());
	m_eavesRoundPrice->setValue(settings.value("roundEavesPrice").toDouble());
	m_planedPrice->setValue(settings.value("planedPrice").toDouble());
	m_fastAdd->setChecked(settings.value("round/fastAdd").toBool());
}

//=============================================================
ConfigurationMailPage::ConfigurationMailPage(QWidget* parent)
	: ConfigurationWidget(parent)
{
	setWindowTitle(tr("E-Mail"));
	createWidgets();
	initialize();
}

//-------------------------------------------------------
// zapisuje wprowadzoną konfiguracje w konfiguracji
void ConfigurationMailPage::save()
{
	QSettings settings;
	settings.setValue("mail/host", m_host->text());
	settings.setValue("mail/port", m_port->value());
	settings.setValue("mail/userName", m_userName->text());
	settings.setValue("mail/password", m_password->text());
	settings.setValue("mail/sender", m_sender->text());
	settings.setValue("mail/sslConnection", m_sslConnection->isChecked());
}

//-------------------------------------------------------
// tworzenie widgetów dla konfiguracji połączenia mail
void ConfigurationMailPage::createWidgets()
{
	m_host = new QLineEdit(this);
	m_port = new QSpinBox(this);
	m_userName = new QLineEdit(this);
	m_password = new QLineEdit(this);
	m_sender = new QLineEdit(this);
	m_sslConnection = new QCheckBox(this);
	m_checkConnectButton = new QPushButton(tr("Test połączenia"), this);
	m_checkStateIcon = new QLabel(this);

	connect(m_checkConnectButton, SIGNAL(clicked()), this, SLOT(checkConnected()));

	m_password->setEchoMode(QLineEdit::Password);	// ukrywanie znaków
	m_port->setRange(0, 65353);						// zakres portów do wyboru

	QLabel* titleLabel = new QLabel(tr("<h2>Wiadomości e-mail</h2>"), this);

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(tr("Serwer SMTP poczty wychodzącej"), m_host);
	formLayout->addRow(tr("Port serwera SMTP"), m_port);
	formLayout->addRow(tr("Nazwa użytkownika (e-mail)"), m_userName);
	formLayout->addRow(tr("Hasło"), m_password);
	formLayout->addRow(tr("Podpis użytkownika"), m_sender);
	formLayout->addRow(tr("Bezpieczne połączenie SSL"), m_sslConnection);

	QHBoxLayout* checkStateLayout = new QHBoxLayout;
	checkStateLayout->addStretch(1);
	checkStateLayout->addWidget(m_checkConnectButton);
	checkStateLayout->addWidget(m_checkStateIcon);
	checkStateLayout->addStretch(1);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(titleLabel);
	mainLayout->addStretch(1);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(checkStateLayout);
	mainLayout->addStretch(1);
}

//-------------------------------------------------------
// wczytuje zapisaną konfiguracje dla poczty e-mail
void ConfigurationMailPage::initialize()
{
	QSettings settings;
	m_host->setText(settings.value("mail/host").toString());
	m_port->setValue(settings.value("mail/port").toInt());
	m_userName->setText(settings.value("mail/userName").toString());
	m_password->setText(settings.value("mail/password").toString());
	m_sender->setText(settings.value("mail/sender").toString());
	m_sslConnection->setChecked(settings.value("mail/sslConnection").toBool());
}

//-------------------------------------------------------
// sprawdza połączenie z serwerem SMTP
void ConfigurationMailPage::checkConnected()
{
	SmtpClient smtp(m_host->text(), m_port->value(),
		m_sslConnection->isChecked() ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

	smtp.setUser(m_userName->text());
	smtp.setPassword(m_password->text());

	bool bSuccess = true;
	if (!smtp.connectToHost())
		bSuccess = false;

	if (!smtp.login())
		bSuccess = false;

	if (bSuccess)
	{
		m_checkStateIcon->setPixmap(QPixmap("icons/ok.png"));
		smtp.quit();
	}
	else m_checkStateIcon->setPixmap(QPixmap("icons/cancel.png"));
}
