#include "addGroupSquaredDialog.h"
#include "groupSquaredDelegate.h"
#include <qlayout.h>
#include <qlabel.h>


AddGroupSquaredDialog::AddGroupSquaredDialog(QWidget* parent)
	: QDialog(parent)
{
	// tworzenie widgetów oraz ustawienie parametrów dialogu
	setWindowTitle(tr("Dodawanie grupowe elementów"));

	createWidgets();
	createShortcut();

	//tworzenie modelu danych
	m_model = new GroupSquaredModel(this);
	m_treeView->setModel(m_model);
	m_treeView->setItemDelegate(new GroupSquaredDelegate(this));

	// tworzenie połączeń sygnałów ze slotami
	connect(m_addItem, SIGNAL(clicked()), this, SLOT(addNewRecord()));
	connect(m_removeItems, SIGNAL(clicked()), m_model, SLOT(removeCheckedItems()));
	connect(m_buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

//---------------------------------------------------------
// tworzenie widgetów używanych w dialogu
void AddGroupSquaredDialog::createWidgets()
{
	m_widthEdit		= new QDoubleSpinBox(this);
	m_heightEdit	= new QDoubleSpinBox(this);
	m_lengthEdit	= new QDoubleSpinBox(this);
	m_countEdit		= new QSpinBox(this);
	m_treeView		= new QTreeView(this);
	m_planed		= new QCheckBox(tr("Strugane"), this);
	m_addItem		= new QPushButton(tr("Dodaj"), this);
	m_removeItems	= new QPushButton(tr("Usuń"), this);
	m_buttons		= new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

	QHBoxLayout* topLayout = new QHBoxLayout;
	topLayout->addWidget(new QLabel(tr("Szerokość [cm]"), this));
	topLayout->addWidget(m_widthEdit);
	topLayout->addWidget(new QLabel(tr("Wysokość [cm]"), this));
	topLayout->addWidget(m_heightEdit);
	topLayout->addWidget(m_planed);

	QHBoxLayout* bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(new QLabel(tr("Długość [m]"), this));
	bottomLayout->addWidget(m_lengthEdit);
	bottomLayout->addWidget(new QLabel(tr("Ilość [szt]"), this));
	bottomLayout->addWidget(m_countEdit);
	bottomLayout->addWidget(m_addItem);
	bottomLayout->addWidget(m_removeItems);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(m_treeView);
	mainLayout->addLayout(bottomLayout);
	mainLayout->addWidget(m_buttons);
}

//----------------------------------------------
// przyspisuje sktóry klawiszowe do opdowiednich
// widgetów
void AddGroupSquaredDialog::createShortcut()
{
	m_addItem->setShortcut(QKeySequence("Return"));	// dodaje nowy rekord (element do listy)
	m_buttons->button(QDialogButtonBox::Ok)->setShortcut(QKeySequence("F10"));	// całkowita akceptacja dialogu
}

//----------------------------------------------
// dodaje nowy element do modelu
void AddGroupSquaredDialog::addNewRecord()
{
	m_model->addSquared(m_widthEdit->value(), m_heightEdit->value(),
		m_lengthEdit->value(), m_countEdit->value(), m_planed->isChecked());

	m_lengthEdit->setFocus();
	m_lengthEdit->selectAll();
}
