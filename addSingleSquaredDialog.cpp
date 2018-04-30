#include "addSingleSquaredDialog.h"
#include <qlayout.h>
#include <qformlayout.h>

AddSingleSquaredDialog::AddSingleSquaredDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Dodawanie pojedynczego elementu"));
	createWidgets();

	connect(m_acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
}

//---------------------------------------------
// tworzenie widgetów
void AddSingleSquaredDialog::createWidgets()
{
	m_width = new QDoubleSpinBox(this);
	m_height = new QDoubleSpinBox(this);
	m_length = new QDoubleSpinBox(this);
	m_count = new QDoubleSpinBox(this);
	m_planed = new QCheckBox(this);
	m_acceptButton = new QPushButton(QIcon("icons/ok.png"), tr("OK"), this);

	m_length->setMaximum(10000);
	m_count->setMaximum(10000);
	m_width->selectAll();

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(tr("Szerokość [cm]"), m_width);
	formLayout->addRow(tr("Wysokość [cm]"), m_height);
	formLayout->addRow(tr("Długość [m]"), m_length);
	formLayout->addRow(tr("Ilość [szt]"), m_count);
	formLayout->addRow(tr("Strugane"), m_planed);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(m_acceptButton);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
}
