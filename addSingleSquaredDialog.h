#ifndef __ADD_SINGLE_SQUARED_DIALOG_H__
#define __ADD_SINGLE_SQUARED_DIALOG_H__

#include <qdialog.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

// <summary>
// dialog wprowadzania pojedynczego elementu
// </summary>
class AddSingleSquaredDialog : public QDialog {
	Q_OBJECT

public:
	AddSingleSquaredDialog(QWidget* parent = 0);

	double width() const { return m_width->value(); }
	double height() const { return m_height->value(); }
	double length() const { return m_length->value(); }
	double count() const { return m_count->value(); }
	bool isPlaned() const { return m_planed->isChecked(); }

private:
	void createWidgets();

	QDoubleSpinBox* m_width;
	QDoubleSpinBox* m_height;
	QDoubleSpinBox* m_length;
	QDoubleSpinBox* m_count;
	QCheckBox* m_planed;
	QPushButton* m_acceptButton;
};

#endif //__ADD_SERVICE_DIALOG_H__
