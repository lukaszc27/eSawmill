#ifndef __CONFIGURATION_WIDGETS_H__
#define __CONFIGURATION_WIDGETS_H__

#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcombobox.h>


// <summary>
// klasa bazowa dla wszystkich widgetów konfiguracji
// klasa abstrakcyjna
// </summary>
class ConfigurationWidget : public QWidget {
	Q_OBJECT

public:
	ConfigurationWidget(QWidget* parent = 0);

public slots:
	virtual void save() { };			// zapis konfiguracji
	virtual void initialize() { };		// odczyt konfiguracji
};

// <summary>
// główna konfiguracja programu
// możliwość edycji danych firmowych oraz loga firmy
// </summary>
class ConfigurationMainPage : public ConfigurationWidget {
	Q_OBJECT

public:
	ConfigurationMainPage(QWidget* parent = 0);

public slots:
	void save();
	void initialize();

private:
	void createWidgets();

	QLabel* m_titleLabel;
	QLineEdit* m_companyName;
	QLineEdit* m_ownName;
	QLineEdit* m_ownSurname;
	QLineEdit* m_nip;
	QLineEdit* m_regon;
	QLineEdit* m_pesel;
	QLineEdit* m_city;
	QLineEdit* m_zipCode;
	QLineEdit* m_zipName;
	QLineEdit* m_street;
	QLineEdit* m_houseNumber;
	QLineEdit* m_phoneNumber;
	QLineEdit* m_mail;
	QLineEdit* m_website;
	QLineEdit* m_bank;			// nazwa banku
	QLineEdit* m_bankNumber;	// nr rachunku bankowego

	QGroupBox*		m_bannerGroup;
	QGroupBox*		m_appStyleGroup;
	QLabel*			m_banner;
	QPushButton*	m_loadButton;
	QComboBox*		m_appStyle;

private slots:
	void loadBannerFromFile();	// wczytuje logo firmowe z pliku i zapisuje ścieszkę w konfiguracji
};

// <summary>
// konfiguracja zamówienia
// </summary>
class ConfigurationOrderPage : public ConfigurationWidget {
	Q_OBJECT

public:
	ConfigurationOrderPage(QWidget* parent = 0);

public slots:
	void save();
	void initialize();

private:
	void createWidgets();

	QDoubleSpinBox* m_priceEdit;
	QCheckBox*		m_fastAdd;
};

// <summary>
// konfiguracja usług
// </summary>
class ConfigurationServicePage : public ConfigurationWidget {
	Q_OBJECT

public:
	ConfigurationServicePage(QWidget* parent = 0);

public slots:
	void save();
	void initialize();

private:
	void createWidgets();

	QDoubleSpinBox* m_roundPrice;
	QDoubleSpinBox* m_eavesRoundPrice;
	QDoubleSpinBox* m_planedPrice;
	QCheckBox*		m_fastAdd;
};

// <summary>
// konfiguracja polączenia do wysyłania poczty mail
// </summary>
class ConfigurationMailPage : public ConfigurationWidget {
	Q_OBJECT

public:
	ConfigurationMailPage(QWidget* parent = 0);

public slots:
	void save();
	void initialize();

private:
	void createWidgets();

	QLineEdit* m_host;
	QSpinBox*  m_port;
	QLineEdit* m_userName;
	QLineEdit* m_password;
	QLineEdit* m_sender;
	QCheckBox* m_sslConnection;
	QPushButton* m_checkConnectButton;
	QLabel* m_checkStateIcon;

private slots:
	void checkConnected();	// sprawdza połączenie z serwerem SMTP
};

#endif //__CONFIGURATION_WIDGETS_H__
