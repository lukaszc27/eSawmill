#include "addRoundDialog.h"
#include <qlayout.h>
#include <qformlayout.h>
#include <qsettings.h>


AddRoundDialog::AddRoundDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Dodawanie kłody"));
	createWidgets();

	connect(m_acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
}

//-------------------------------------------
// tworzenie widgetów
void AddRoundDialog::createWidgets()
{
	m_diameter		= new QDoubleSpinBox(this);
	m_length		= new QDoubleSpinBox(this);
	m_checkBox		= new QCheckBox(this);
	m_acceptButton	= new QPushButton(QIcon("icons/ok.png"), tr("OK"), this);

	m_diameter->selectAll();

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(tr("Średnica [cm]"), m_diameter);
	formLayout->addRow(tr("Długość [m]"), m_length);
	formLayout->addRow(tr("Kantowana"), m_checkBox);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(m_acceptButton);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
}
