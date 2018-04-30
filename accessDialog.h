#ifndef __ACCESSDIALOG_H__
#define __ACCESSDIALOG_H__

#include <qdialog.h>
#include <qlineedit.h>
#include <qdialogbuttonbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qformlayout.h>

// <summary>
// Pobranie danych do nawiązania połączenia z bazą danych
// </summary>
class AccessDialog : public QDialog {
	Q_OBJECT

public:
	AccessDialog(QWidget* parent = 0);

	QString userName() const;
	QString password() const;
	QString hostName() const;
	
	bool rememberMe() const;
private:
	// funkcja tworząca widgety w oknie
	void createWidgets();

	// widgety
	QLineEdit*			m_hostName;
	QLineEdit*			m_userName;
	QLineEdit*			m_password;
	QCheckBox*			m_rememberMe;
	QDialogButtonBox*	m_buttons;
};

#endif //__ACCESSDIALOG_H__
