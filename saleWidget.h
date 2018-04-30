#ifndef __SALE_WIDGET_H__
#define __SALE_WIDGET_H__

#include <qwidget.h>
#include <qgroupbox.h>
#include <qtreeview.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qsortfilterproxymodel.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>

#include "contrahentWidget.h"
#include "squaredModel.h"
#include "articleModel.h"
#include "articleDelegate.h"


// <summary>
// widget umożliwiający sprzedaż produktów tartacznych
// kożysta z modelu SquaredModel
// </summary>
class SaleWidget : public QWidget {
	Q_OBJECT

public:
	SaleWidget(QWidget* parent = 0);

private:
	void createWidgets();		// tworzenie widgetów używanych w oknie
	void createShortcut();		// tworzy skróty
	
	QWidget* createElementTab();	// tworzy zakładkę elementów konstrukcyjncyh
	QWidget* createArticleTab();	// tworzy zakładkę artykułów

	QGroupBox* m_contrahentGroup;	// grupa kontrahenta
	QGroupBox* m_saleGroup;			// grupa informacji o sprzedarzy
	QTabWidget*  m_tabWidget;		// zakładka tabWidget

	// grupa elementów konstrukcyjnych
	QTreeView*		m_treeView;
	QPushButton*	m_addButton;
	QPushButton*	m_removeButton;
	QPushButton*	m_optionButton;
	QPushButton*	m_printButton;
	QPushButton*	m_duplicateButton;
	QLabel*			m_planedOfMetric;
	QLabel*			m_sumOfMetric;
	QLabel*			m_toPay;

	// grupa artykułów
	QTreeView*		m_articleView;
	QPushButton*	m_addArticle;
	QPushButton*	m_removeArticle;
	QComboBox*		m_articleFilterColumn;
	QLineEdit*		m_articleFilterText;
	QLabel*			m_toPayLabel;

	// grupa kontrahent
	ContrahentWidget* m_contrahentWidget;

	// grupa płatności
	QDateEdit*		m_saleDay;
	QDateEdit*		m_dayToPay;
	QComboBox*		m_payOption;
	QSpinBox*		m_numberOfDay;
	QDoubleSpinBox* m_price;
	QSpinBox*		m_rabat;
	QCheckBox*		m_vatCheck;
	QComboBox*		m_vatValue;

	// model danych
	SquaredModel* m_model;
	ArticleModel* m_articleModel;
	QSortFilterProxyModel* m_articleProxyModel;

private slots:
	void importFromXML();
	void exportToXML();
	void addNewElement();							// dodaje nowy element do listy
	void changePayOptions(int index);				// uaktywnia lub dezaktywuje pewne widgety płatności
	void changeDateToPay(int value);				// dodaje ilość dni do daty płatności
	void print();									// drukuje rozliczenie (paragon)
	void updateAllWidgets();						// aktualizuje wszystkie widgety w zależności od modelu
	void duplicateSelectedItem();					// duplikowanie zaznaczonego elementu
	void setVatEnabled(int state);					// aktywuje lub zezaktywuje funkcje dodawania VAT-u
	void setVatValue(const QString& value);
	void addArticle();								// wywołuje dialog wyboru artykułu
	void addArticle(int id);						// dodaje wybrany artykuł do listy
	void filterArticlesItems(const QString& text);	// filtruje elementy po zadanych kryteriach
};

#endif //__SALE_WIDGET_H__
