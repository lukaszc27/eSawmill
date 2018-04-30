#include "addMagazineDialog.h"
#include <qformlayout.h>
#include <qlayout.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>


AddMagazineDialog::AddMagazineDialog(QWidget* parent, bool updateMode, int id)
	: QDialog(parent)
{
	createWidgets();

	if (updateMode)
	{
		setWindowTitle(tr("Aktualizacja informacji o magazynie"));
		loadFromDatabase(id);
	}
	else
	{
		setWindowTitle(tr("Dodawanie nowego magazynu"));
	}

	connect(m_acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
}

//---------------------------------------
// wczytuje dane z serwera MySQL
bool AddMagazineDialog::loadFromDatabase(int id)
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen() && db.isValid())
	{
		QSqlQuery query;
		query.prepare("SELECT nazwa, opis FROM artykuly_magazyn WHERE idMagazynu = ?");
		query.bindValue(0, id);

		if (query.exec() && query.first())
		{
			m_name->setText(query.value(0).toString());
			m_descprition->document()->setPlainText(query.value(1).toString());

			return true;
		}
	}
	return false;
}

//---------------------------------------
// tworzy widgety dodatkowe
void AddMagazineDialog::createWidgets()
{
	m_name			= new QLineEdit(this);
	m_descprition	= new QPlainTextEdit(this);
	m_acceptButton	= new QPushButton(QIcon("icons/ok.png"), tr("OK"), this);

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(tr("Nazwa"), m_name);
	formLayout->addRow(tr("Opis"), m_descprition);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(m_acceptButton);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
}
