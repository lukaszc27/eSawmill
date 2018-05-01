#ifndef __CONFIGURATION_DIALOG_H__
#define __CONFIGURATION_DIALOG_H__

#include <qdialog.h>
#include <qlistwidget.h>
#include <qstackedwidget.h>
#include <qtabwidget.h>
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

	QTabWidget* m_tabWidget;
	QDialogButtonBox*	m_buttons;

	// widgety konfiguracji
	ConfigurationMainPage* m_mainPage;
	ConfigurationOrderPage* m_orderPage;
	ConfigurationServicePage* m_servicePage;
	ConfigurationMailPage* m_mailPage;
};

#endif //__CONFIGURATION_DIALOG_H__
