#include "dateFilterDialog.h"
#include <qlabel.h>
#include <qlayout.h>

DateFilterDialog::DateFilterDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Filtorwanie według daty"));
	createWidgets();

	connect(m_acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
}

//-----------------------------------------
// tworzy widgety w oknie
void DateFilterDialog::createWidgets()
{
	m_filterGroup	= new QGroupBox(tr("Filtorwanie wg. daty"), this);
	m_columnGroup	= new QGroupBox(tr("Kolumna"), this);
	m_addColumn		= new QRadioButton(tr("Data przyjęcia"), this);
	m_endColumn		= new QRadioButton(tr("Termin realizacji"), this);
	m_firstDate		= new QDateEdit(this);
	m_lastDate		= new QDateEdit(this);
	m_acceptButton	= new QPushButton(QIcon("icons/ok.png"), tr("OK"), this);

	// ustawiamy dodakowe opcje widgetów
	m_filterGroup->setCheckable(true);
	m_firstDate->setDate(QDate::currentDate());
	m_firstDate->setCalendarPopup(true);
	m_lastDate->setDate(QDate::currentDate());
	m_lastDate->setCalendarPopup(true);

	m_filterGroup->setChecked(false);	// początkowo filter dat wyłączony
	m_addColumn->setChecked(true);		// początkowo data dodania domyślnie zaznaczona

	QHBoxLayout* columnLayout = new QHBoxLayout(m_columnGroup);
	columnLayout->addWidget(m_addColumn);
	columnLayout->addWidget(m_endColumn);

	QHBoxLayout* dateLayout = new QHBoxLayout;
	dateLayout->addWidget(new QLabel(tr("Od"), this));
	dateLayout->addWidget(m_firstDate);
	dateLayout->addWidget(new QLabel(tr("Do"), this));
	dateLayout->addWidget(m_lastDate);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(m_acceptButton);

	QVBoxLayout* filterLayout = new QVBoxLayout(m_filterGroup);
	filterLayout->addWidget(m_columnGroup);
	filterLayout->addLayout(dateLayout);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(m_filterGroup);
	mainLayout->addLayout(buttonLayout);
}
