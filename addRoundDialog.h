#ifndef __ADD_ROUND_DIALOG_H__
#define __ADD_ROUND_DIALOG_H__

#include <qdialog.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>


// <summary>
// dodawanie kłody do usługi
// </summary>
class AddRoundDialog : public QDialog {
	Q_OBJECT

public:
	AddRoundDialog(QWidget* parent = 0);

	double diameter() const { return m_diameter->value(); }
	double length() const { return m_length->value(); }
	bool isEaves() const { return m_checkBox->isChecked(); }

private:
	void createWidgets();

	QDoubleSpinBox*	m_diameter;
	QDoubleSpinBox* m_length;
	QCheckBox*		m_checkBox;
	QPushButton*	m_acceptButton;
};

#endif //__ADD_ROUND_DIALOG_H__
