#ifndef __SERVICE_MANAGER_H__
#define __SERVICE_MANAGER_H__

#include <qwidget.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtreeview.h>

#include <qsortfilterproxymodel.h>
#include "serviceModel.h"
#include "ordersProxyModel.h"
#include "dateFilterDialog.h"
#include "myTreeView.h"



// <summary>
// zarządzanie usługami (cięcie drewna dostarczonego)
// </summary>
class ServiceManager : public QWidget {
	Q_OBJECT

public:
	ServiceManager(QWidget* parent = 0);

private:
	void createWidgets();
	void createShortcut();

	// wyszukiwarka usług
	QComboBox*		m_filterOption;
	QLineEdit*		m_filterText;
	QPushButton*	m_filterButton;
	
	// wyświetlanie oraz operacje na usługach
	MyTreeView*		m_treeView;
	QPushButton*	m_addButton;
	QPushButton*	m_removeButton;
	QPushButton*	m_dateFilterButton;

	ServiceModel*			m_model;
	OrdersProxyModel*		m_dateProxyModel;

	// dialogi
	DateFilterDialog* m_dateFilterDialog;

private slots:
	void addNewService();
	void editService(const QModelIndex& index);
	void filterService();						// wyszukiwanie usług po podanych kryteriach
	void filterServiceByDate();					// filtruje usługi według daty
};

#endif //__SERVICE_MANAGER_H__
