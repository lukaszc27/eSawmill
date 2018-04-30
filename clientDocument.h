#ifndef __CLIENT_DOCUMENT_H__
#define __CLIENT_DOCUMENT_H__

#include <qtextdocument.h>
#include <qdatetime.h>

// <summary>
// podstawowy szablon dokumentu rozliczenia wręczany klientowi
// </summary>
class ClientDocument : public QTextDocument {
	Q_OBJECT

public:
	ClientDocument(QObject* parent = nullptr);

	QString companyBlock();	// wyświetla blok przedstawiający dane firmy

	void setDocumentDate(const QDate& date) { m_documentDate = date; }				// ustawia datę wystawienia dokumentu
	void setDocumentTitle(const QString& title) { m_documentTitle = title; }		// ustawia tytuł dokumentu
	void prepareToPrint(const QString contrahentBlock, 
		const QString itemsBlock,
		const QString other = QString());											// przygotowuje dokument do druku
private:
	QString m_companyName;	// nazwa firmy
	QString m_ownName;		// imię właściciela
	QString m_ownSurname;	// nazwisko właściciela
	QString m_nip;			// nip firmy
	QString m_regon;		// regon
	QString m_pesel;		// pesel (raczej nie dotyczy)
	QString m_city;			// miejsce prowadzenia działalności
	QString m_zipCode;		// kod pocztowy
	QString m_zipName;		// poczta
	QString m_street;		// ulica
	QString m_houseNumber;	// numer domu/mieszkania
	QString m_phoneNumber;	// numer telefonu
	QString m_mail;			// mail firmowy
	QString m_website;		// strona firmowa

	// dane dla pojedynczego egzemplarza dokumentu
	QString m_documentTitle;	// tytuł dokumentu
	QDate	m_documentDate;		// data wystawienia dokumentu
};

#endif //__CLIENT_DOCUMENT_H__
