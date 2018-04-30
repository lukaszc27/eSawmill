#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <qmainwindow.h>
#include <qsqldatabase.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qmdiarea.h>
#include <qmdisubwindow.h>
#include <qaction.h>

#include "mdiArea.h"

// <summary>
// Okno główne aplikacji
// nawiązuje połączenia z bazą danych MySQL
// </summary>
class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);

private:
	QSqlDatabase	m_db;			// uchwyt na bazę danych
	MdiArea*		m_mdiArea;		// kontener zarządzania oknami dodatkowymi

	void createWidgets();			// widgety dla użytkowników zalogowanych do basy danych

	// główne menu
	QMenu* m_mainMenu;
	QMenu* m_contrahentMenu;
	QMenu* m_magazineMenu;
	QMenu* m_ordersMenu;
	QMenu* m_serviceMenu;

	// akcje
	QAction* m_saleAction;
	QAction* m_lock;
	QAction* m_unlock;

protected:
	void closeEvent(QCloseEvent* Event);	// zdarzenie zamknięcia okna

private slots:
	void connectToDatabase();		// nawiązuje połączenie z bazą danych
	void unlockFunctions();			// odblokowuje funkcje po zalogowaniu
	void lockFunctions();			// blokuje funkcje dla niezalogowanych użytkowników oraz wylogowuje z MySQL
	void openContrahentManager();	// otwiera okno zarządzania kontrahentami w głównym kontenerze
	void openOrdersManager();		// otwiera okno zarządzania zamówieniami w głównym kontenerze
	void openSaleWindow();			// otwirra okno umożliwiające sprzedaż elementów
	void openServiceManager();		// otwiera okno zarządzania usługami
	void openConfigurationDialog();	// uruchamia dialog konfiguracji programu
	void openArticlesManager();		// otwiera okno zarządzania artykułami
	void openMagazineManager();		// otwiera okno zarządzania magazynami artykułów
	
	// otwiera dialog analizy i optymalizacji zamówień
	void openAnaliseAndOptimalizeOrders();
};

#endif //__MAIN_WINDOW_H__
