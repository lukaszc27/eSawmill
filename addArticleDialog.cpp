#include "addArticleDialog.h"
#include <qgroupbox.h>
#include <qformlayout.h>
#include <qlayout.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qmessagebox.h>


AddArticleDialog::AddArticleDialog(QWidget* parent, const int articleId, bool updateMode)
	: QDialog(parent)
	, m_articleId(articleId)
	, m_updateMode(updateMode)
{
	m_db = QSqlDatabase::database();

	setWindowTitle(tr("Dodawanie nowego artykułu"));

	createWidgets();
	
	// jeśli jest to tryb aktualizacji wczytujemy dane z bazy
	if (updateMode)
	{
		setWindowTitle(tr("Aktualizacja wybranego artykułu"));
		loadArticleFromDatabase();
	}

	connect(m_netto, SIGNAL(valueChanged(double)), this, SLOT(calculateBruttoFromNetto(double)));
	connect(m_vat, SIGNAL(currentIndexChanged(QString)), this, SLOT(calculateBruttoFromVat(QString)));
	connect(m_buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));
	connect(m_brutto, SIGNAL(valueChanged(double)), this, SLOT(calculateDetalPrice(double)));
	connect(m_marza, SIGNAL(valueChanged(double)), this, SLOT(calculateDetalFromMarza(double)));
}

//-------------------------------------------------
// zwraca liczbę VAT
double AddArticleDialog::vat() const
{
	QString vatValue = m_vat->currentText();
	vatValue.remove(" %");

	return vatValue.toDouble();
}

//-------------------------------------------------
// tworzenie widgetów
void AddArticleDialog::createWidgets()
{
	m_magazine	= new QComboBox(this);
	m_jm		= new QComboBox(this);
	m_name		= new QLineEdit(this);
	m_netto		= new QDoubleSpinBox(this);
	m_brutto	= new QDoubleSpinBox(this);
	m_detalPrice = new QDoubleSpinBox(this);
	m_marza		= new QDoubleSpinBox(this);
	m_vat		= new QComboBox(this);
	m_buttons	= new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	
	// ustawiamy wartości domyślne
	m_buttons->button(QDialogButtonBox::Ok)->setIcon(QIcon("icons/ok.png"));
	m_buttons->button(QDialogButtonBox::Cancel)->setIcon(QIcon("icons/cancel.png"));

	m_netto->setMaximum(10000);
	m_brutto->setMaximum(10000);
	m_detalPrice->setMaximum(10000);

	QStringList vatValues;
	vatValues << tr("0 %") << tr("3 %") << tr("5 %") << tr("8 %") << tr("23 %");
	m_vat->addItems(vatValues);

	QStringList jmValues;
	jmValues << tr("mb") << tr("m2") << tr("m3") << tr("szt");
	m_jm->addItems(jmValues);

	// wczytanie listy magazynów z bazy
	{
		QSqlDatabase db = QSqlDatabase::database();
		if (db.isValid() && db.isOpen())
		{
			QSqlQuery q;
			if(q.exec("select idMagazynu, nazwa from artykuly_magazyn"))
			{
				while (q.next())
					m_magazine->addItem(q.value(1).toString(), q.value(0).toInt());
			}
			else
			{
				QMessageBox::critical(this, tr("Błąd"), q.lastError().text());
			}
		}
	}

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(tr("Magazyn"), m_magazine);
	formLayout->addRow(tr("Nazwa"), m_name);
	formLayout->addRow(tr("Jednostka miary"), m_jm);

	QGroupBox* priceGroup = new QGroupBox(tr("Cena"), this);
	QFormLayout* priceLayout = new QFormLayout(priceGroup);
	priceLayout->addRow(tr("Wartość netto [zł]"), m_netto);
	priceLayout->addRow(tr("VAT"), m_vat);
	priceLayout->addRow(tr("Wartość brutto [zł]"), m_brutto);
	priceLayout->addRow(tr("Marża [%]"), m_marza);
	priceLayout->addRow(tr("Cena detaliczna [zł]"), m_detalPrice);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(formLayout);
	mainLayout->addWidget(priceGroup);
	mainLayout->addWidget(m_buttons);
}

//-------------------------------------------------
// wczytuje podany artykuł do edycji
void AddArticleDialog::loadArticleFromDatabase()
{
	if (m_db.isOpen() && m_db.isValid())
	{
		QSqlQuery q;
		q.prepare("SELECT artykuly_magazyn.nazwa, artykuly.nazwa, cenaNetto, cenaBrutto, vat, jm, cenaDetaliczna, marza "
			"FROM artykuly, artykuly_magazyn WHERE idArtykulu = ? AND artykuly.idMagazynu = artykuly_magazyn.idMagazynu");
		q.bindValue(0, m_articleId);

		if (q.exec())
		{
			if (q.first())
			{
				m_magazine->setCurrentText(q.value(0).toString());
				m_name->setText(q.value(1).toString());
				m_netto->setValue(q.value(2).toDouble());
				m_brutto->setValue(q.value(3).toDouble());
				m_jm->setCurrentText(q.value(5).toString());
				m_detalPrice->setValue(q.value(6).toDouble());
				m_marza->setValue(q.value(7).toDouble());

				QString vatValue = tr("%1 %").arg(q.value(4).toString());
				m_vat->setCurrentText(vatValue);
			}
		}
		else
		{
			QMessageBox::critical(this, tr("Błąd"),
				q.lastError().text());

			return;
		}
	}
}

//-------------------------------------------------
// wywoływane przy zmiawnie wartości netto
void AddArticleDialog::calculateBruttoFromNetto(double value)
{
	QString vatValue = m_vat->currentText();
	vatValue.remove(" %");
	double vat = 100 + vatValue.toDouble();

	double brutto = (value*vat) / 100;
	m_brutto->setValue(brutto);
}

//-------------------------------------------------
// wywoływane w przypadku zmiany wartości VAT
void AddArticleDialog::calculateBruttoFromVat(const QString& value)
{
	QString vatValue = value;
	vatValue.remove(" %");
	double vat = 100 + vatValue.toDouble();
	
	double brutto = (m_netto->value()*vat) / 100;
	m_brutto->setValue(brutto);
}

//-------------------------------------------------
// wywoływane przy zmianie wartoścu brutto
void AddArticleDialog::calculateDetalPrice(double value)
{
	double brutto = value;
	double marza = 100 + m_marza->value();
	double detal = (brutto*marza) / 100;

	m_detalPrice->setValue(detal);
}

//-------------------------------------------------
// oblicza cenę detaliczną po zmianie marży
void AddArticleDialog::calculateDetalFromMarza(double value)
{
	double brutto = m_brutto->value();
	double marza = 100 + value;
	double price = (brutto*marza) / 100;

	m_detalPrice->setValue(price);
}
