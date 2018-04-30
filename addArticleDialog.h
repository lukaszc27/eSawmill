#ifndef __ADD_ARTICLE_DIALOG_H__
#define __ADD_ARTICLE_DIALOG_H__

#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>


// <summary>
// dodawanie nowego artykułu do bazy
// </summary>
class AddArticleDialog : public QDialog {
	Q_OBJECT

public:
	AddArticleDialog(QWidget* parent = 0, const int articleId = 0, bool updateMode = false);

	QString name() const { return m_name->text().toUpper(); }
	QString jm() const { return m_jm->currentText(); }
	double nettoPrice() const { return m_netto->value(); }
	double bruttoPrice() const { return m_brutto->value(); }
	double vat() const;
	double detalPrice() const { return m_detalPrice->value(); }
	double marza() const { return m_marza->value(); }
	int magazineId() const { return m_magazine->currentData(Qt::UserRole).toInt(); }
	int articleId() const { return m_articleId; }

private:
	void createWidgets();
	void loadArticleFromDatabase();	// wczytuje określony artykuł do edycji

	int m_articleId;				// id artykułu przeznaczonego do aktualizacji
	bool m_updateMode;				// dialog uruchomiony w trybie aktualizacji danych
	QSqlDatabase m_db;				// uchyt na połączenie z bazą danych

	QLineEdit* m_name;
	QComboBox* m_magazine;
	QComboBox* m_jm;
	QDoubleSpinBox* m_netto;
	QDoubleSpinBox* m_brutto;
	QDoubleSpinBox* m_detalPrice;
	QDoubleSpinBox* m_marza;
	QComboBox* m_vat;
	QDialogButtonBox* m_buttons;

private slots:
	void calculateBruttoFromNetto(double value);		// wywoływane przy zmianie wartości netto
	void calculateBruttoFromVat(const QString& value);	// wywoływane prz zmianie wartości VAT
	void calculateDetalPrice(double value);				// wywoływane po zmianie wartości brutto
	void calculateDetalFromMarza(double value);			// oblicza cenę detaliczną po zmianie marży
};

#endif //__ADD_ARTICLE_DIALOG_H__
