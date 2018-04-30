#ifndef __ADD_ORDER_DIALOG_H__
#define __ADD_ORDER_DIALOG_H__

#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qgroupbox.h>
#include <qdatetimeedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtreeview.h>
#include <qcombobox.h>
#include <qaction.h>
#include <qlabel.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qspinbox.h>
#include <qtextedit.h>
#include <qradiobutton.h>

#include "contrahentWidget.h"
#include "squaredModel.h"
#include "elementsFilter.h"
#include "articleModel.h"
#include "articleRecord.h"

// <summary>
// dialog dodawania nowego zamówienia
// </summary>
class AddOrderDialog : public QDialog {
	Q_OBJECT

public:
	AddOrderDialog(QWidget* parent = 0, bool updateMode = false, int orderId = 0);

	QDate addDate() const { return m_addDateEdit->date(); }	// data przyjęcia zamówienia
	QDate endDate() const { return m_endDateEdit->date(); }	// termin realizacji
	QDate nearDate();	// pobiera ostatnią date z bazy i dolicza czas do realizacji

	QString title() const { return m_titleEdit->text().toUpper(); }	// tytuł zamówienia/kosztorysu
	QString notes() const { return m_notesEdit->document()->toPlainText(); }

	bool isDone() const { return m_done->isChecked(); }				// informacja czy zamówienie zostało zrealizowane
	bool loadOrderFromDatabase();									// wczytuje zamówienie z bazy (np. do edycji)

	// zwraca id aktualnie używanego kontrahenta
	int contrahentId() const { return m_contrahentWidget->contrahentId(); }
	int rabat() const { return m_discountEdit->value(); }
	int priorytet() const { return m_priotytetEdit->currentIndex(); }
	QString vat() const { return m_vatEdit->currentText(); }
	double price() const { return m_price->value(); }
	double calculateMainPrice();		// obliczenie całkowitej kwoty do zapłaty za zamówienie (uwzględniając rabat)


	QList<SquaredRecord> listOfItems() const { return m_model->listOfItems(); }				// zwraca listę elementów dodanych w zamówieniu
	QList<ArticleRecord> articlesItems() const { return m_articleModel->listOfItems(); }	// zwraca listę artykułów dodanych do zamówienia

private:
	void createWidgets();	// tworzy wszystkie dodatkowe widgety wymagane w oknie
	void createShortcut();	// tworzy skróty klawiszowe
	
	QWidget* mainWidget();		// tworzy widgety dla głównej zakładki
	QWidget* elementsWidget();	// widgety zarządzania elementami
	QWidget* articleWidget();	// widgety artyków dodanych do zamówienia

	QPushButton*		m_acceptButton;
	QPushButton*		m_rejectButton;
	QPushButton*		m_printButton;		// drukowanie zamówienia
	QPushButton*		m_mailSendButton;	// wysyłanie kosztorysu zamówienia przez email

	// główne kontenery grupujące widgety w oknie
	QGroupBox* m_contrahentGroup;
	QGroupBox* m_orderGroup;
	QGroupBox* m_notesGroup;

	// widgety wprowadzania danych
	ContrahentWidget* m_contrahentWidget;	// wybór kontrahenta z bazy

	// widgety wprowadzania
	QLineEdit* m_titleEdit;
	QDateEdit* m_addDateEdit;
	QDateEdit* m_endDateEdit;
	QSpinBox* m_discountEdit;	// rabat
	QComboBox* m_vatEdit;
	QCheckBox* m_vatEnable;
	QCheckBox* m_done;
	QTextEdit* m_notesEdit;
	QComboBox* m_priotytetEdit;
	QDoubleSpinBox* m_price;

	// grupa elementów
	QTreeView*		m_itemsView;
	QPushButton*	m_addButton;
	QPushButton*	m_removeButton;
	QPushButton*	m_duplicateButton;
	QLabel*			m_sumLabel;
	QLabel*			m_planedLabel;
	QLabel*			m_priceLabel;

	QLabel*			m_toPayLabel;	// etykieta wyświetlająca całkowity koszt do zapłaty

	// grupa artykułów
	QTreeView*		m_articleView;
	QPushButton*	m_addArticle;
	QPushButton*	m_removeArticle;
	QLabel*			m_articlePriceLabel;

	// widgety filtorwania kolumn
	QDoubleSpinBox* m_minValue;
	QDoubleSpinBox* m_maxValue;
	QRadioButton* m_widthColumn;
	QRadioButton* m_heightColumn;
	QRadioButton* m_lengthColumn;
	QGroupBox* m_sizeFilterGroup;
	QPushButton* m_rangeFilterButton;

	// akcje dla przycisku dodaj
	QAction* m_addAction;
	QAction* m_addGroupAction;

	// model reprezentacji danych
	SquaredModel* m_model;							// główny model elementów zamówienia (kantówek)
	ElementsFilterModel* m_elementsProxyModel;		// model filtorwania głównego modelu
	ArticleModel* m_articleModel;					// model obsługi artykułów dodanych do zamówienia

	int m_currentId;		// id aktualnie aktualizowanego zamówienia (wykożystywane tylko w trybie aktualizacji)
	bool m_updateMode;		// flaga trybu aktualizacji

private slots:
	void addGroupItems();			// dodawanie grupowe elementów
	void addSingleItem();			// dodaje pojedynczy element do listy
	void print();					// drukuje kosztorys (fakturę za zamówienie)
	void updateAllWidgets();		// aktualizuje stan wszystkich widgeów w zależności od potrzeb modelu
	void exportItemsToFile();		// zapisuje wprowadzone elementy w pliku XML
	void importItemsFromFile();		// importuje listę elementów z pliku
	void filterByRange();			// filtruje wiersze według określonych danych
	void duplicateSelectedItem();	// powielda zaznaczony rekord
	void vatEnabled(int state);		// aktywuje lub dezaktywuje wybór vatu
	void activatedVatValue(const QString& text);	// ustawienie aktywowanej wartości do modelu
	void addNewArticle();			// wywołuje dialog wyboru artykułu
	void addNewArticle(int id);		// dodaje nowy artykuł do zamówienia
	void sendByMailButton();		// wysłanie kosztorysu przez adres mail
};

#endif //__ADD_ORDER_DIALOG_H__
