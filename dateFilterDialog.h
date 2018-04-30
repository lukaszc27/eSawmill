#ifndef __DATE_FILTER_DIALOG_H__
#define __DATE_FILTER_DIALOG_H__

#include <qdialog.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qdatetimeedit.h>
#include <qpushbutton.h>


// <summary>
// ustawienie dat do do filtorwania zamówien oraz usług w menadżerach
// </summary>
class DateFilterDialog : public QDialog {
	Q_OBJECT

public:
	DateFilterDialog(QWidget* parent = 0);

	QDate firstDate() const {
		return m_firstDate->date();
	}

	QDate lastDate() const {
		return m_lastDate->date();
	}

	bool filterEnabled() const {
		return m_filterGroup->isChecked();
	}

	// według której kolumny przeszukiwać
	// 0 - data dodania
	// 1 - termin realizacji
	int filterColumn() const {
		if (m_addColumn->isChecked())
			return 0;
		else if(m_endColumn->isChecked())
		return 1;
	}

private:
	// tworzy dodatkowe widgety w oknie
	void createWidgets();

	// kontrolki
	QGroupBox*		m_columnGroup;
	QGroupBox*		m_filterGroup;
	QDateEdit*		m_firstDate;
	QDateEdit*		m_lastDate;
	QRadioButton*	m_addColumn;
	QRadioButton*	m_endColumn;
	QPushButton*	m_acceptButton;
};

#endif //__DATE_FILTER_DIALOG_H__
