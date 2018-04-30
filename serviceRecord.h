#ifndef __SERVICE_RECORD_H__
#define __SERVICE_RECORD_H__

#include <qobject.h>
#include <qdatetime.h>

class ServiceRecord
{
public:
	ServiceRecord() { };
	ServiceRecord(const QString title,
		const QString company,
		const QString name,
		const QString surname,
		const QDate addDate,
		const QDate endDate,
		const int id,
		const int priorytet,
		const bool done,
		const bool checked = false)
	{
		setTitle(title);
		setCompany(company);
		setName(name);
		setSurname(surname);
		setAddDate(addDate);
		setEndDate(endDate);
		setDone(done);
		setPriorytet(priorytet);
		setId(id);
		setChecked(checked);
	}

	void setTitle(const QString title) { m_title = title; }
	void setCompany(const QString company) { m_company = company; }
	void setName(const QString name) { m_name = name; }
	void setSurname(const QString surname) { m_surname = surname; }
	void setAddDate(const QDate date) { m_addDate = date; }
	void setEndDate(const QDate date) { m_endDate = date; }
	void setDone(const bool done) { m_done = done; }
	void setChecked(const bool checked) { m_checked = checked; }
	void setId(const int id) { m_id = id; }
	void setPriorytet(const int priorytet) { m_priorytet = priorytet; }

	QString title() const { return m_title; }
	QString company() const { return m_company; }
	QString name() const { return m_name; }
	QString surname() const { return m_surname; }
	QDate addDate() const { return m_addDate; }
	QDate endDate() const { return m_endDate; }
	bool isDone() const { return m_done; }
	bool isChecked() const { return m_checked; }
	int id() const { return m_id; }
	int priorytet() const { return m_priorytet; }

private:
	QString m_title;
	QString m_company;
	QString m_name;
	QString m_surname;
	QDate m_addDate;
	QDate m_endDate;
	bool m_done;
	bool m_checked;
	int m_id;
	int m_priorytet;
};

#endif //__SERVICE_RECORD_H__
