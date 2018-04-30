#ifndef __ORDER_ANALISE_AND_OPTIMIZE_H__
#define __ORDER_ANALISE_AND_OPTIMIZE_H__

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>


// <summary>
// okno analizy wszystkich zamówien oraz optymalizacji
// (optymalizacja usuwa zrealizowane zamówienia w celu przyspieszczenia
// działania programu)
// </summary>
class OrdersAnalizeAndOptimize : public QDialog {
	Q_OBJECT

public:
	OrdersAnalizeAndOptimize(QWidget* parent = 0);

private slots:
	void analise();			// analiza rekorów w bazie
	void optimalise();		// optymalizacja bazy
private:
	void createWidgets();

	QLabel* m_allOrdersInDatabaseLabel;	// ilość wszystkich zamówień w bazie danych
	QLabel*	m_realizeOrdersLabel;		// ilość zamówień zrealizowaych
	QLabel*	m_ordersToRealizeLabel;		// zamówienia oczekujące na zrealizowanie
	QLabel* m_lostTermOrdersLabel;		// zamówienia po czasie realizacji

	// to samo co u góry tylko zmienne
	double m_allOrdersInDatabase;
	double m_realizeOrders;
	double m_ordersToRealize;
	double m_lostTermOrders;

	QPushButton* m_acceptButton;
	QPushButton* m_optimilizeButton;
};

#endif //__ORDER_ANALISE_AND_OPTIMIZE_H__
