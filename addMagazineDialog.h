#ifndef __ADD_MAGAZINE_DIALOG_H__
#define __ADD_MAGAZINE_DIALOG_H__

#include <qdialog.h>
#include <qlineedit.h>
#include <qplaintextedit.h>
#include <qpushbutton.h>

// <summary>
// dialog dodawania nowego magazynu do programu
// </summary>
class AddMagazineDialog : public QDialog {
	Q_OBJECT

public:
	AddMagazineDialog(QWidget* parent = 0, bool updateMode = false, int id = 0);

	// wczytuje dane z serwera MySQL po id
	bool loadFromDatabase(int id);

	QString name() const { return m_name->text().toUpper(); }
	QString description() const { return m_descprition->document()->toPlainText().toUpper(); }

private:
	void createWidgets();

	// widgety
	QLineEdit*		m_name;
	QPlainTextEdit* m_descprition;
	QPushButton*	m_acceptButton;
};

#endif //__ADD_MAGAZINE_DIALOG_H__
