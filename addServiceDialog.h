#ifndef __ADD_SERVICE_DIALOG_H__
#define __ADD_SERVICE_DIALOG_H__

#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qtabwidget.h>

#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qtextedit.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qtreeview.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qlabel.h>
#include <qtextstream.h>
#include <qcombobox.h>

#include "contrahentWidget.h"
#include "squaredModel.h"
#include "roundModel.h"
#include "elementsFilter.h"
#include "articleModel.h"


// <summary>
// Dialog dodawania nowej usługi do systemu
// </summary>
class AddServiceDialog : public QDialog {
	Q_OBJECT

public:
	AddServiceDialog(QWidget* parent = 0, const bool updateMode = false, const int serviceId = 0);

	// pobranie wartości wprowadzonywch do dialogu
	QString title() const { return m_title->text().toUpper(); }
	QString notes() const { return m_notesEdit->document()->toPlainText(); }
	QDate addDate() const { return m_addDate->date(); }
	QDate endDate() const { return m_endDate->date(); }

	double rabat() const { return m_rabat->value(); }
	double calculateFullPrice();	// wylicza całkowity koszt do zapłaty razem z doliczeniem rabatu

	bool isDone() const { return m_done->isChecked(); }
	int contrahentId() const { return m_contrahentWidget->contrahentId(); }
	int vat() const;
	int priorytet() const { return m_priorytet->currentIndex(); }

	QList<SquaredRecord> squaredItemsOfList() const { return m_elementsModel->listOfItems(); }
	QList<RoundRecord> roundItemsOfList() const { return m_roundModel->listOfItems(); }
	QList<ArticleRecord> articlesOfList() const { return m_articleModel->listOfItems(); }

public slots:
	void printRounds();		// drukuje spis kłód
	void printSquards();	// drukuje spis elementów do wycięcia
	void sendByMail();		// wysyła kosztorys usługi przez maila

private:
	void createWidgets();
	void createShortcut();

	void setNearDate();				// pobiera najwcześniejszą date z bazy oraz ustawia ja w odpowiedniej kontrolce
	bool loadFromDatabase(int id);	// wczytuje usługe z bazy danych

	QTabWidget* m_tabWidget;
	QPushButton* m_acceptButton;
	QPushButton* m_rejectButton;
	QPushButton* m_sendByMailButton;
	QPushButton* m_printRound;
	QLabel*		 m_fullPriceLabel;	// etykieta wyświetlająca całkowity koszt usługi

	// widgety dla głównej zakładki
	QGroupBox*		m_contrahentGroup;
	QGroupBox*		m_serviceGroup;
	QGroupBox*		m_notesGroup;
	QLineEdit*		m_title;
	QDateEdit*		m_addDate;
	QDateEdit*		m_endDate;
	QDoubleSpinBox* m_rabat;
	QCheckBox*		m_done;
	QTextEdit*		m_notesEdit;
	ContrahentWidget* m_contrahentWidget;
	QCheckBox*		m_vatCheck;
	QComboBox*		m_vatValue;
	QComboBox*		m_priorytet;

	// widgety dla zakładki "kłody"
	QTreeView* m_roundView;	// wyświetla kłody
	QPushButton* m_addRound;
	QPushButton* m_removeRound;
	QPushButton* m_optionRound;
	QLabel* m_sumLabel;
	QLabel* m_priceLabel;
	QPushButton* m_duplicateRoundButton;

	// widgety dla zakładki "Elementy"
	QTreeView* m_elementsView;
	QPushButton* m_addElement;
	QPushButton* m_removeElement;
	QPushButton* m_optionElement;
	QPushButton* m_printElement;
	QPushButton* m_duplicateButton;

	QGroupBox* m_lengthFilterGroup;
	QPushButton* m_lengthFilterButton;
	QDoubleSpinBox* m_minLength;
	QDoubleSpinBox* m_maxLength;

	// widgety dla zakładki "Artykuły"
	QTreeView* m_articleView;
	QPushButton* m_addArticle;
	QPushButton* m_removeArticle;
	QLineEdit* m_articleFilterText;
	QComboBox* m_articleFilterColumn;
	QLabel* m_toPayLabel;

	// filtorwanie elementów
	QGroupBox* m_rangeFilterGroup;
	QRadioButton* m_widthColumn;
	QRadioButton* m_heightColumn;
	QRadioButton* m_lengthColumn;
	QDoubleSpinBox* m_widthValue;
	QDoubleSpinBox* m_heightValue;
	QPushButton* m_rangeFilterButton;

	QWidget* mainWidget();		// tworzy zakładkę (głównych informacji o usłudze)
	QWidget* roundWidget();		// tworzy zakładkę kłód
	QWidget* elementWidget();	// tworzy zakładkę dla elementów
	QWidget* articleWidget();	// twirzy zakładkę dla artykułów

	// modele danych
	SquaredModel*			m_elementsModel;		// zarządzanie listą elementów
	RoundModel*				m_roundModel;			// zarządzanie listą kłód
	ElementsFilterModel*	m_elementsFilterModel;	// filtorwanie według wskazanych karegorii
	ArticleModel*			m_articleModel;
	QSortFilterProxyModel*	m_articleProxyModel;	// model filtorwania artykułów

private slots:
	void addSingleElement();
	void addGroupElement();
	void addRound();								// dodaje kolejną kłode
	void importFromFile();							// wczytuje elementy zapisane w pliku
	void exportToFile();							// zapisuje listę elementów do pliku
	void importRoundsFromFile();					// wczytuje kłody z pliku
	void exportRoundsToFile();						// zapisuje kłody do pliku
	void updateAllWidgets();						// aktualizuje stan wszystkich widgetów na podstawie modelu
	void rangeFilterButtonClicked();				// filtorwanie wierszy według wskazanych kategorii
	void lengthFilterButtonClicked();				// filtrowanie elementów konstrukcyjncych według długości
	void duplicateSelectedItem();					// powiela zaznaczony element
	void duplicateRoundSelectedItems();
	void vatCheckState(int state);					// aktywuje lub dezaktywuje doliczanie vatu
	void setVatValue(const QString& value);
	void addArticle();								// wywoł uje menadżera artykułów w trybie wyboru
	void addArticle(int id);						// dodawanie do listy wybranego artykułu
	void filterArticles(const QString& text);
};

#endif //__ADD_SERVICE_DIALOG_H__
