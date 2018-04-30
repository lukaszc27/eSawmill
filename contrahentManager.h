#ifndef __CONTRAHENT_MANAGER_H__
#define __CONTRAHENT_MANAGER_H__

#include <qdialog.h>
#include <qtreeview.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include "contrahentModel.h"
#include "myTreeView.h"
#include <qsortfilterproxymodel.h>
#include <qsql.h>


class ContrahentManager : public QDialog {
	Q_OBJECT

public:
	ContrahentManager(QWidget* parent = 0, bool selectedMode = false);	// selectedMode - tryb wyboru kontrahenta do
																		//				  zamówienia

private:
	void createWidgets();	// tworzymy wszystkie widgety dla okna

	// pasek filtrowania listy
	QComboBox*		m_filterOption;
	QLineEdit*		m_filterText;
	QPushButton*	m_filterButton;

	// przyciski sterowania daialogiem
	QPushButton*	m_addButton;
	QPushButton*	m_removeButton;

	MyTreeView*		m_treeView;	// element wiświetlający dane pobrane z bazy

	// modele danych
	ContrahentModel*		m_model;		// główny model pobierający dane
	QSortFilterProxyModel*	m_proxyModel;	// model filtorwania danych

	bool m_selectedMode;	// dialog w trybie wyboru kontrahenta

public slots:
	void addNewContrahent();							// dodajemy nowego kontrahenta do bazy
	void filterContrahents();							// wyszukujemy kontrahenta po wybranych opcjach
	void listDoubleClicked(const QModelIndex& index);	// podwujne kliknięcie na listę QTreeView 
														// (emitowanie sygnału z id wybranego kontrahenta)

signals:
	void contrahentSelected(int id);	// sygnał emitowany gdy kontrahent zostanie wybrany
};

#endif //__CONTRAHENT_MANAGER_H__
