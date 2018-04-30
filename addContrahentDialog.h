#ifndef __ADD_CONTRAHENT_DIALOG_H__
#define __ADD_CONTRAHENT_DIALOG_H__

#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qgridlayout.h>


// <summary>
// Dialog wprodadzania danych potrzebnych do dodania nowego kontrahenta do bazy
// </summary>
class AddContrahentDialog : public QDialog {
	Q_OBJECT

public:
	AddContrahentDialog(QWidget* parent = 0);

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

private:
	void createWidgets();		// tworzenie wszystkich widgetów w oknie
	void createVialidators();	// tworzy walidatory danych do QLineEdit

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

	// sterowanie dialogiem
	QDialogButtonBox* m_buttons;
};

#endif //__ADD_CONTRAHENT_DIALOG_H__
