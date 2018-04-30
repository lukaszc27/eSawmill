#ifndef __ORDER_RECORD_H__
#define __ORDER_RECORD_H__

#include <qobject.h>
#include <qdatetime.h>

// <summary>
// reprezentacja danych w pojedynczym wierszu modelu
// </summary>
class OrderRecord
{
public:
	OrderRecord() {};
	OrderRecord(const QString title,
		const QString companyName,
		const QString name,
		const QString surname,
		const QDate addDate,
		const QDate endDate,
		const int rabat,
		bool done)
	{
		setTitle(title);
		setCompanyName(companyName);
		setName(name);
		setSurname(surname);
		setAddDate(addDate);
		setEndDate(endDate);
		setDone(done);
		setRabat(rabat);
		setChecked(false);
	}

	// ustawianie wartoœci
	void setAddDate(const QDate date) { m_addDate = date; }	// nadanie daty przyjêcia
	void setEndDate(const QDate date) { m_endDate = date; }	// nadanie daty zakoñczenia (termin)
	void setTitle(const QString title) { m_title = title; }	// nadanie tytu³u
	void setCompanyName(const QString company) { m_companyName = company; }	// nazwa firmy zamawiaj¹cej
	void setName(const QString name) { m_name = name; }						// imiê osoby zamawiaj¹cej
	void setSurname(const QString surname) { m_surname = surname; }			// nazwisko osoby zamawiaj¹cej
	void setDone(bool done) { m_done = done; }				// zmiana statusu zamówienia
	void setChecked(bool checked) { m_checked = checked; }	// zmiania statusu zaznaczenia
	void setId(int id) { m_id = id; }						// nadanie id zamówieniu
	void setRabat(int rabat) { m_rabat = rabat < 0 ? rabat : 0; }	// ustawia nowy rabat
	void setPriotytet(int priotytet) { m_priotytet = priotytet; }

	// pobieranie wartoœci
	QDate addDate() const { return m_addDate; }
	QDate endDate() const { return m_endDate; }
	QString title() const { return m_title; }
	QString companyName() const { return m_companyName; }
	QString name() const { return m_name; }
	QString surname() const { return m_surname; }
	bool isDone() const { return m_done; }
	bool isChecked() const { return m_checked; }
	int id() const { return m_id; }
	int rabat() const { return m_rabat; }
	int priotytet() const { return m_priotytet; }

private:
	QString m_title;		// tytu³ zamówienia
	QString	m_companyName;	// firma zamawiaj¹ca
	QString m_name;			// imiê osoby zamawiaj¹cej
	QString m_surname;		// nazwisko osoby zamawiaj¹cej
	QDate	m_addDate;		// data przyjêcia
	QDate	m_endDate;		// termin realizacji
	bool	m_done;			// czy zakoñczone
	bool	m_checked;		// czy zaznaczone
	int		m_id;			// id zamówienia
	int		m_rabat;		// rabat
	int		m_priotytet;
};

#endif //__ORDER_RECORD_H__
