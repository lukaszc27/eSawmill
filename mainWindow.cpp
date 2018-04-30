#include "mainWindow.h"
#include <qmessagebox.h>
#include <qapplication.h>
#include <qsqldatabase.h>
#include <qsqldriver.h>
#include <qsqldriverplugin.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qstatusbar.h>
#include <qevent.h>
#include <qsettings.h>
#include <qlistwidget.h>
#include <qtimer.h>

#include "accessDialog.h"
#include "ordersManager.h"
#include "contrahentManager.h"
#include "saleWidget.h"
#include "serviceManager.h"
#include "addOrderDialog.h"
#include "ordersAnaliseAndOptimize.h"
#include "configurationDialog.h"
#include "articlesManager.h"
#include "magazineManager.h"


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	setWindowIcon(QIcon("icons/mainIcon.png"));
	m_db = QSqlDatabase::addDatabase("QMYSQL3");
	
	// tworzymy widgety w oknie
	createWidgets();

	// blokujemy program dla nie zalogowanych użytkowników
	lockFunctions();
}

//--------------------------------------------------
// tworzy widgety dla okna głównego
void MainWindow::createWidgets()
{
	// utworzenie głównego kontenera zarządzania oknai
	m_mdiArea = new MdiArea(this);
	m_mdiArea->setViewMode(QMdiArea::ViewMode::SubWindowView);

	setCentralWidget(m_mdiArea);

	// tworzenie paska menu i statusBar
	QMenuBar* menuBar		= new QMenuBar(this);
	QStatusBar* statusBar	= new QStatusBar(this);
	
	// tworzenie menu
	m_mainMenu			= new QMenu(QApplication::applicationName(), menuBar);
	m_contrahentMenu	= new QMenu(tr("Kontrahenci"), menuBar);
	m_magazineMenu		= new QMenu(tr("Magazyn"), menuBar);
	m_ordersMenu		= new QMenu(tr("Zamówienia"), menuBar);
	m_serviceMenu		= new QMenu(tr("Usługi"), menuBar);

	m_lock = new QAction(QIcon("icons/lock.png"), tr("Zablokuj"), this);
	m_unlock = new QAction(QIcon("icons/unlock.png"), tr("Zaloguj"), this);

	connect(m_lock, SIGNAL(triggered()), this, SLOT(lockFunctions()));
	connect(m_unlock, SIGNAL(triggered()), this, SLOT(connectToDatabase()));

	// główne menu programu
	m_mainMenu->addAction(QIcon("icons/configuration.png"), tr("Konfiguracja"),
		this, SLOT(openConfigurationDialog()));

	m_mainMenu->addSeparator();
	m_mainMenu->addAction(m_lock);
	m_mainMenu->addAction(m_unlock);
	m_mainMenu->addSeparator();
	m_mainMenu->addAction(QIcon("icons/exit.png"), tr("Zakończ"), this, SLOT(close()));

	// akcje dla podmenu "kontrahenci"
	QAction* contrahentManagerAction = new QAction(QIcon("icons/contrahents.png"),
		tr("Zarządzanie kontrahentami"), this);
	m_contrahentMenu->addAction(contrahentManagerAction);

	// akcje dla podmenu "magazyn"
	m_magazineMenu->addAction(tr("Zarządzanie magazynami"), this, SLOT(openMagazineManager()));
	m_magazineMenu->addAction(tr("Artykuły"), this, SLOT(openArticlesManager()));

	// akcje dla podmenu "zamówienia"
	QAction* ordersManagerAction = new QAction(QIcon("icons/order.png"), tr("Zarządzanie zamówieniami"), this);
	m_ordersMenu->addAction(ordersManagerAction);
	m_ordersMenu->addAction(tr("Analiza"), this, SLOT(openAnaliseAndOptimalizeOrders()));

	// podmenu dla usług
	m_serviceMenu->addAction(QIcon("icons/services.png"), tr("Zarządzanie usługami"), this, SLOT(openServiceManager()));

	// akcja sprzedaży elementów konstrukcyjnych
	m_saleAction = new QAction(tr("Sprzedaż"), this);
	connect(m_saleAction, SIGNAL(triggered()), this, SLOT(openSaleWindow()));

	menuBar->addMenu(m_mainMenu);
	menuBar->addMenu(m_contrahentMenu);
	menuBar->addMenu(m_magazineMenu);
	menuBar->addMenu(m_ordersMenu);
	menuBar->addMenu(m_serviceMenu);
	menuBar->addAction(m_saleAction);


	setMenuBar(menuBar);		// ustawiamy menu dla okna głównego
	setStatusBar(statusBar);	// ustawiamy pasek statusu dla okna

	// połączenia akcji z odpowiednimi slotami
	connect(contrahentManagerAction, SIGNAL(triggered()), this, SLOT(openContrahentManager()));
	connect(ordersManagerAction, SIGNAL(triggered()), this, SLOT(openOrdersManager()));
}

//--------------------------------------------------
// odblokowuje program dla zalogowanych użytkowników
void MainWindow::unlockFunctions()
{
	m_contrahentMenu->setEnabled(true);
	m_magazineMenu->setEnabled(true);
	m_ordersMenu->setEnabled(true);
	m_serviceMenu->setEnabled(true);
	m_saleAction->setEnabled(true);

	// zmieniamy infoamcje o zalogowaniu
	m_mdiArea->setLocked(false);
	m_unlock->setEnabled(false);
	m_lock->setEnabled(true);
}

//--------------------------------------------------
// blokuje funkcje programu, oraz jeśli jest ktoś zalogowany
// do MySQL wylogowuje go
void MainWindow::lockFunctions()
{
	m_contrahentMenu->setEnabled(false);
	m_magazineMenu->setEnabled(false);
	m_ordersMenu->setEnabled(false);
	m_serviceMenu->setEnabled(false);
	m_saleAction->setEnabled(false);

	// zmiana inforamcji o zalogowaniu
	m_mdiArea->setLocked(true);
	m_unlock->setEnabled(true);
	m_lock->setEnabled(false);

	if (m_db.isOpen() && m_db.isValid())
		m_db.close();

	// wywołujemy dialog logowania do bazy
	QTimer::singleShot(0, this, SLOT(connectToDatabase()));
}

//--------------------------------------------------
// zdarzenie zamknięcia okna
void MainWindow::closeEvent(QCloseEvent * Event)
{
#if _DEBUG
	Event->accept();
#else
	int iRet = QMessageBox::question(this, tr("Zapytanie"),
		tr("Czy napewno chcesz zakończyć prace z programem %1").arg(QApplication::applicationName()),
		QMessageBox::Yes | QMessageBox::No);

	if (iRet == QMessageBox::Yes)
		Event->accept();
	else Event->ignore();
#endif
}

//--------------------------------------------------
void MainWindow::connectToDatabase()
{
	AccessDialog* accessDialog = new AccessDialog(this);
	if (accessDialog->exec() == AccessDialog::Accepted)
	{
#if _DEBUG
		m_db.setHostName("localhost");
		m_db.setUserName("root");
		m_db.setPassword("bGA6HWrR");
#else
		m_db.setHostName(accessDialog->hostName());
		m_db.setUserName(accessDialog->userName());
		m_db.setPassword(accessDialog->password());
#endif
		m_db.setDatabaseName("sawmill");	// wybranie bazy danych

		if (accessDialog->rememberMe())
		{
			QSettings settings;
			settings.setValue("access/host", accessDialog->hostName());
			settings.setValue("access/userName", accessDialog->userName());
			settings.setValue("access/password", accessDialog->password());
			settings.setValue("access/rememberMe", accessDialog->rememberMe());
		}

		// prubujemy nawiązać połączenie z bazą danych
		if (!m_db.open())
		{
			if (m_db.lastError().number() == -1)
			{
				// obsługa błędu nie załadowania sterownika bazy danych
				QStringList avaribleDrivers = QSqlDatabase::drivers();

				// pokazujemy listę z dostępnymi sterownikami
				QListWidget* listWidget = new QListWidget;
				listWidget->addItems(avaribleDrivers);

				QStringList libraryPaths = QApplication::libraryPaths();
				listWidget->addItems(libraryPaths);

				listWidget->show();
			}
			else
			{
				QMessageBox::critical(this, QString::number(m_db.lastError().number()), m_db.lastError().text());
				return;
			}
		}
		else {
			unlockFunctions();
		}
	}
}

//--------------------------------------------------
// otwiera okno zarządzania zamówieniami w
// główym kontenerze okna
void MainWindow::openOrdersManager()
{
	OrdersManager* manager = new OrdersManager(this);
	QMdiSubWindow* widget = m_mdiArea->addSubWindow(manager);
	widget->setWindowIcon(manager->windowIcon());
	widget->setWindowTitle(manager->windowTitle());
	
	widget->show();
}

//--------------------------------------------------
// otwiera okno umożliwiające sprzedaż elementów
void MainWindow::openSaleWindow()
{
	SaleWidget* saleWindow = new SaleWidget(this);
	QMdiSubWindow* widget = m_mdiArea->addSubWindow(saleWindow);
	widget->setWindowIcon(saleWindow->windowIcon());
	widget->setWindowTitle(saleWindow->windowTitle());

	widget->show();
}

//--------------------------------------------------
// otwira okno do zarządzania zamówieniami
void MainWindow::openServiceManager()
{
	ServiceManager* manager = new ServiceManager(this);
	QMdiSubWindow* widget = m_mdiArea->addSubWindow(manager);
	widget->setWindowTitle(manager->windowTitle());
	widget->setWindowIcon(manager->windowIcon());

	widget->show();
}

//--------------------------------------------------
// otwiera dialog do konfiguracji programu
void MainWindow::openConfigurationDialog()
{
	ConfigurationDialog* dialog = new ConfigurationDialog(this);
	dialog->exec();
}

//--------------------------------------------------
// otwiera dialog zarządzania artykułami
void MainWindow::openArticlesManager()
{
	ArticlesManager* manager = new ArticlesManager(this);
	QMdiSubWindow* widget = m_mdiArea->addSubWindow(manager);
	widget->setWindowTitle(manager->windowTitle());
	widget->setWindowIcon(manager->windowIcon());
	widget->show();
}

//--------------------------------------------------
// otwiera dialog zarządzania magazynem artykułów
void MainWindow::openMagazineManager()
{
	MagazineManager* manager = new MagazineManager(this);
	QMdiSubWindow* window = m_mdiArea->addSubWindow(manager);
	window->setWindowTitle(manager->windowTitle());
	window->setWindowIcon(manager->windowIcon());

	window->show();
}

//--------------------------------------------------
// otwiera dialog analizy i optymalizacji zamówień
void MainWindow::openAnaliseAndOptimalizeOrders()
{
	OrdersAnalizeAndOptimize* dialog = new OrdersAnalizeAndOptimize(this);
	dialog->exec();
}

//--------------------------------------------------
// otwiera okno zarządzania kontrahentami w
// głównym kontenerze okna
void MainWindow::openContrahentManager()
{
	ContrahentManager* manager	= new ContrahentManager(this);
	QMdiSubWindow* widget = m_mdiArea->addSubWindow(manager);
	widget->setWindowTitle(manager->windowTitle());
	widget->setWindowIcon(manager->windowIcon());

	widget->show();
}
