#ifndef __ORDERS_MANAGER_H__
#define __ORDERS_MANAGER_H__

#include <qwidget.h>
#include <qdialogbuttonbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qtreeview.h>
#include "ordersProxyModel.h"
#include "ordersModel.h"
#include "dateFilterDialog.h"
#include "myTreeView.h"
#include "realizedOrdersFilter.h"


// <summary>
// zarządzanie zamówieniami
// do prawidłowego działania potrzebuje komponentu contrahentManager
// </summary>
class OrdersManager : public QWidget {
	Q_OBJECT

public:
	OrdersManager(QWidget* parent = nullptr);

private:
	void createWidgets();	// tworzymy widgety używane w oknie
	void createShortcut();	// przypisuje sktóty klawiszowe akcjom

	QComboBox*		m_filterOption;
	QLineEdit*		m_filterText;
	QPushButton*	m_filterButton;
	MyTreeView*		m_treeView;
	QPushButton*	m_addButton;
	QPushButton*	m_removeButton;
	QPushButton*	m_dateFilterButton;
	QGroupBox*		m_optionsGroup;
	QCheckBox*		m_afterRealizeDate;			// wyświetlanie zamówień po terminie realizacji
	QCheckBox*		m_realizedFilterChecked;	// wyświetlanie zamówień zrealizowanych 

	// modele danych
	OrdersProxyModel*		m_proxyModel;			// model filtrowania
	RealizedOrdersFilter*	m_realizedFilterModel;	// model odfiltorwania zamówień zrealizowanych
	OrdersModel*			m_model;				// główny model danych

	// dialogi pomocnicze
	DateFilterDialog* m_dateFilterDialog;

private slots:
	void addNewOrder();						// dodaje nowe zamówienie i odświeża listę
	void editOrder(const QModelIndex& index);	// wczytuje zamówienie po id do edycji
	void filterOrders();						// filtruje zamówienia według podanych kryteriów
	void dateFilterOption();
};

#endif //__ORDERS_MANAGER_H__
