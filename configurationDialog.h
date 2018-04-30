#ifndef __CONFIGURATION_DIALOG_H__
#define __CONFIGURATION_DIALOG_H__

#include <qdialog.h>
#include <qlistwidget.h>
#include <qstackedwidget.h>
#include <qdialogbuttonbox.h>

#include "configurationWidgets.h"


// <summary>
// główna konfiguracja programu
// </summary>
class ConfigurationDialog : public QDialog {
	Q_OBJECT

public:
	ConfigurationDialog(QWidget* parent = 0);

	void accept();

private:
	void createWidgets();
	void createWidgetsForMenu();

	QStackedWidget*		m_stackedWidget;
	QListWidget*		m_listWidget;
	QDialogButtonBox*	m_buttons;

	// widgety dla menu konfiguracji
	QListWidgetItem* m_mainMenuItem;
	QListWidgetItem* m_ordersMenuItem;
	QListWidgetItem* m_serviceMenuItem;
	QListWidgetItem* m_mailMenuItem;

	// widgety konfiguracji
	ConfigurationMainPage* m_mainPage;
	ConfigurationOrderPage* m_orderPage;
	ConfigurationServicePage* m_servicePage;
	ConfigurationMailPage* m_mailPage;

private slots:
	void changePage(QListWidgetItem* current, QListWidgetItem* previous);
};

#endif //__CONFIGURATION_DIALOG_H__
