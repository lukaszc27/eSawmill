#ifndef __CONTRAHENT_WIDGET_H__
#define __CONTRAHENT_WIDGET_H__

#include <qwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>


// <summary>
// umożliwia wybór kontrahenta do wybranego dokumentu
// </summary>
class ContrahentWidget : public QWidget {
	Q_OBJECT

public:
	ContrahentWidget(QWidget* parent = 0);

	// bezpieczne pobieranie wartości wprowadzonych przez użytkownika
	// wszystkie funkcje zwracają tekst zamieniony na wielkie litery
	QString name() const;
	QString surname() const;
	QString companyName() const;
	QString nip() const;
	QString regon() const;
	QString pesel() const;
	QString zipCode() const;
	QString zipName() const;
	QString city() const;
	QString street() const;
	QString houseNumber() const;
	QString phone() const;
	QString mail() const;
	QString website() const;
	QString contrahentAddress() const;

	// zwraca id aktualnego kontrahenta
	int contrahentId() const { return m_contrahentId; }

private:
	QSqlDatabase m_db;		// uchwyt na połączenie z bazą MySQL
	void createWidgets();	// tworzenie wszystkich widgetów w oknie

	// dane kontrahenta
	QLineEdit* m_name;
	QLineEdit* m_surname;
	QLineEdit* m_companyName;
	QLineEdit* m_nip;
	QLineEdit* m_regon;
	QLineEdit* m_pesel;

	// dane adresowe
	QLineEdit* m_zipCode;
	QLineEdit* m_zipName;
	QLineEdit* m_city;
	QLineEdit* m_street;
	QLineEdit* m_houseNumber;
	QLineEdit* m_phone;
	QLineEdit* m_mail;
	QLineEdit* m_website;

	QPushButton* m_chooseButton;	// przycisk wyboru kontrahenta

	int m_contrahentId;	// id wybranego kontrahenta

private slots:
	void openContrahentManager();	// otwiera menadżera kontrahentów w trybie wyboru

public slots:
	void loadContrahent(int id);	// wczytuje kontrahenta o podanym ID
};

#endif //__CONTRAHENT_WIDGET_H__
