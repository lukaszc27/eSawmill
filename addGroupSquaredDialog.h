#ifndef __ADD_GROUP_SQUARED_DIALOG_H__
#define __ADD_GROUP_SQUARED_DIALOG_H__

#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtreeview.h>
#include <qlist.h>
#include "groupSquaredModel.h"
#include "squaredRecord.h"


class AddGroupSquaredDialog : public QDialog {
	Q_OBJECT

public:
	AddGroupSquaredDialog(QWidget* parent = 0);

	// pobieramy listę danych z modelu w celu puźniejszego przesłania do okna głównego
	QList<SquaredRecord> itemsOfList() const { return m_model->itemsOfList(); }

private:
	void createWidgets();	// tworzenie widgetów
	void createShortcut();	// przypisuje skróty klawiszowe

	// wskaźniki na widgety w oknie
	QDoubleSpinBox*		m_widthEdit;
	QDoubleSpinBox*		m_heightEdit;
	QDoubleSpinBox*		m_lengthEdit;
	QSpinBox*			m_countEdit;
	QCheckBox*			m_planed;
	QPushButton*		m_addItem;
	QPushButton*		m_removeItems;
	QTreeView*			m_treeView;

	// sterowanie dialogiem
	QDialogButtonBox*	m_buttons;

	// model danych
	GroupSquaredModel* m_model;

private slots:
	void addNewRecord();	// dodaje nowy element do modelu
};

#endif //__ADD_GROUP_SQUARED_DIALOG_H__
