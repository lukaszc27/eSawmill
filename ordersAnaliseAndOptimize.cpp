#include "ordersAnaliseAndOptimize.h"
#include <qformlayout.h>
#include <qlayout.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qvariant.h>
#include <qdatetime.h>
#include <qmessagebox.h>


OrdersAnalizeAndOptimize::OrdersAnalizeAndOptimize(QWidget* parent)
	: QDialog(parent)
{
	m_allOrdersInDatabase = m_realizeOrders = m_ordersToRealize = m_lostTermOrders = 0;

	setMinimumSize(400, 300);
	createWidgets();

	connect(m_optimilizeButton, SIGNAL(clicked()), this, SLOT(optimalise()));
	connect(m_acceptButton, SIGNAL(clicked()), this, SLOT(accept()));

	analise();
}

//---------------------------------------------
// optymalizuje bazę danych pod względem szybkości
// działania
void OrdersAnalizeAndOptimize::optimalise()
{
	QMessageBox::information(this, tr("Informacja"),
		tr("Optymalizacja usuwa z bazy zamówienia zrealizowane "
			"oraz nadaje priorytet wysoki zamówieniom które są już po dacie realizacji"));

	int iRet = QMessageBox::warning(this, tr("Ostrzeżenie"),
		tr("Operacja usuwania zrealizowanych zamówień nie może być cofnięta!\n"
			"Czy mimo to chcesz kontynuować?"),
		QMessageBox::Yes | QMessageBox::No);

	if (iRet == QMessageBox::Yes)
	{
		QSqlDatabase db = QSqlDatabase::database();
		if (db.isOpen() && db.isValid())
		{
			// usunięcie zrealizowanych zamówień
			QSqlQuery q;
			if(q.exec("select zamowienieId from zamowienia where zrealizowane = 1"))
				while (q.next())
				{
					// usuwamy
					QSqlQuery query;
					query.prepare("call UsunZamowienie(?)");
					query.bindValue(0, q.value(0).toInt());
					if (!query.exec())
					{
						QMessageBox::critical(this, tr("Błąd"), query.lastError().text());
						break;
					}
				}

			// zmiana priorytetu dla zamówień nie zrealizowanych po upłynięicu terminu realizacji
			q.clear();
			if(q.exec("select zamowienieId from zamowienia where zrealizowane = 0 and terminRealizacji <= current_date()"))
				while (q.next())
				{
					QSqlQuery query;
					query.prepare("update zamowienia set priorytet = 2 where zamowienieId = ?");
					query.bindValue(0, q.value(0).toInt());
					if (!query.exec())
					{
						QMessageBox::critical(this, tr("Błąd"), query.lastError().text());
						break;
					}
				}
		}
	}
	analise();
}

//---------------------------------------------
// tworzy widgety w oknie
void OrdersAnalizeAndOptimize::createWidgets()
{
	m_optimilizeButton = new QPushButton(tr("Optymalizuj"), this);
	m_acceptButton = new QPushButton(QIcon("icons/ok.png"), tr("OK"), this);
	m_allOrdersInDatabaseLabel = new QLabel(this);
	m_realizeOrdersLabel = new QLabel(this);
	m_ordersToRealizeLabel = new QLabel(this);
	m_lostTermOrdersLabel = new QLabel(this);

	QLabel* titleLabel = new QLabel(tr("<h3>Analiza i optymalizacja zamówień</h3>"));

	QHBoxLayout* titleLayout = new QHBoxLayout;
	titleLayout->addStretch(1);
	titleLayout->addWidget(titleLabel);
	titleLayout->addStretch(1);

	QFormLayout* form = new QFormLayout;
	form->addRow(tr("Wszystkie zamówienia w bazie danych"), m_allOrdersInDatabaseLabel);
	form->addRow(tr("Zrealizowane"), m_realizeOrdersLabel);
	form->addRow(tr("Oczekujące na zrealizowanie"), m_ordersToRealizeLabel);
	form->addRow(tr("Zamówienia niezrealizowane po dacie realizacji"), m_lostTermOrdersLabel);

	QHBoxLayout* formCentre = new QHBoxLayout;
	formCentre->addStretch(1);
	formCentre->addLayout(form);
	formCentre->addStretch(1);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_optimilizeButton);
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(m_acceptButton);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(titleLayout);
	mainLayout->addStretch(1);
	mainLayout->addLayout(formCentre);
	mainLayout->addStretch(2);
	mainLayout->addLayout(buttonLayout);
}

//---------------------------------------------
// analizuje rekordy w bazie danych
void OrdersAnalizeAndOptimize::analise()
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen() && db.isValid())
	{
		QSqlQuery q;

		// pobranie liczby rekordów z bazy danych
		if (q.exec("select count(zamowienieId) from zamowienia"))
			if (q.first())
				m_allOrdersInDatabase = q.value(0).toDouble();

		// pobranie ilości zamówień zrealizowanych
		q.clear();
		if (q.exec("select count(zamowienieId) from zamowienia where zrealizowane = 1"))
			if (q.first())
				m_realizeOrders = q.value(0).toDouble();

		// pobranie ilości zamówień po terminie realizacji
		q.clear();
		if(q.exec("select terminRealizacji from zamowienia where zrealizowane = 0"))
			while (q.next())
			{
				if (q.value(0).toDate() <= QDate::currentDate())
					m_lostTermOrders++;
			}

		// pobranie ilości zamówien do realizacji
		q.clear();
		if (q.exec("select count(zamowienieId) from zamowienia where zrealizowane = 0"))
			if (q.first())
				m_ordersToRealize = q.value(0).toDouble();
	}
	m_allOrdersInDatabaseLabel->setText(QString::number(m_allOrdersInDatabase));

	m_realizeOrdersLabel->setText(tr("%1 (%2%)")
		.arg(m_realizeOrders)
		.arg((m_realizeOrders / m_allOrdersInDatabase) * 100));

	m_ordersToRealizeLabel->setText(tr("%1 (%2%)")
		.arg(m_ordersToRealize)
		.arg((m_ordersToRealize / m_allOrdersInDatabase) * 100));

	m_lostTermOrdersLabel->setText(tr("%1 (%2%)")
		.arg(m_lostTermOrders)
		.arg((m_lostTermOrders / m_allOrdersInDatabase) * 100));

}
