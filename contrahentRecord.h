#ifndef __CONTRAHENT_RECORD_H__
#define __CONTRAHENT_RECORD_H__

#include <qobject.h>

// <summary>
// Reprezentacja pojedynczego wiersza w modelu (QTreeView)
// </summary>
class ContrahentRecord
{
public:
	ContrahentRecord() {};	// konstruktor domyœlny

	ContrahentRecord(QString name,
		QString surname,
		QString companyName,
		QString nip,
		QString regon,
		QString pesel,
		QString city,
		QString zipCode,
		QString zipName,
		QString street,
		QString houseNumber,
		QString phone,
		QString mail,
		QString website)
	{
		setName(name);
		setSurname(surname);
		setNip(nip);
		setRegon(regon);
		setPesel(pesel);
		setCity(city);
		setZipCode(zipCode);
		setZipName(zipName);
		setStreet(street);
		setHouseNumber(houseNumber);
		setPhone(phone);
		setMail(mail);
		setWebsite(website);
	}

	void setName(const QString newName) { m_name = newName; }
	void setSurname(const QString surname) { m_surname = surname; }
	void setCompanyName(const QString name) { m_companyName = name; }
	void setNip(const QString nip) { m_nip = nip; }
	void setRegon(const QString regon) { m_regon = regon; }
	void setPesel(const QString pesel) { m_pesel = pesel; }
	void setCity(const QString city) { m_city = city; }
	void setZipCode(const QString zipCode) { m_zipCode = zipCode; }
	void setZipName(const QString zipName) { m_zipName = zipName; }
	void setStreet(const QString street) { m_street = street; }
	void setHouseNumber(const QString houseNumber) { m_houseNumber = houseNumber; }
	void setPhone(const QString phone) { m_phone = phone; }
	void setMail(const QString mail) { m_mail = mail; }
	void setWebsite(const QString website) { m_website = website; }
	void setId(int id) { m_id = id; }
	void setChecked(bool checked) { m_checked = checked; }
	void setDebator(bool debator) { m_debator = debator; }

	QString name() const { return m_name; }
	QString surname() const { return m_surname; }
	QString companyName() const { return m_companyName; }
	QString nip() const { return m_nip; }
	QString regon() const { return m_regon; }
	QString pesel() const { return m_pesel; }
	QString city() const { return m_city; }
	QString zipCode() const { return m_zipCode; }
	QString zipName() const { return m_zipName; }
	QString street() const { return m_street; }
	QString houseNumber() const { return m_houseNumber; }
	QString phone() const { return m_phone; }
	QString mail() const { return m_mail; }
	QString website() const { return m_website; }
	
	int id() const { return m_id; }
	bool isChecked() const { return m_checked; }
	bool isDebator() const { return m_debator; }

private:
	QString m_name;
	QString m_surname;
	QString m_companyName;
	QString m_nip;
	QString m_regon;
	QString m_pesel;
	QString m_city;
	QString m_zipCode;
	QString m_zipName;
	QString m_street;
	QString m_houseNumber;
	QString m_phone;
	QString m_mail;
	QString m_website;

	// to ju¿ nie bie¿e udzia³u w wyœwietlaniu ale jest pomocne podczas pracy z modelem
	int m_id;
	bool m_checked;
	bool m_debator;
};

#endif //__CONTRAHENT_RECORD_H__
