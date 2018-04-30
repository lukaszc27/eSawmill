#ifndef __ARTICLE_RECORD_H__
#define __ARTICLE_RECORD_H__

#include <qobject.h>

// <summary>
// opisuje pojedynczy rekord z bazy artykułów
// </summary>
class ArticleRecord
{
public:
	ArticleRecord()
	{
		m_checked = false;
	}

	void setId(int id) { m_id = id; }
	void setMagazineId(int id) { m_idMagazine = id; }
	void setName(const QString name) { m_name = name; }
	void setNettoPrice(double price) { m_priceNetto = price; }
	void setBruttoPrice(double price) { m_priceBrutto = price; }
	void setVat(double vat) { m_vat = vat; }
	void setJm(QString jm) { m_jm = jm; }
	void setChecked(bool checked) { m_checked = checked; }
	void setDetalPrie(double price) { m_detalPrice = price; }
	void setMarza(double marza) { m_marza = marza; }
	void setCount(double count) { m_count = count; }

	int id() const { return m_id; }
	int magazineId() const { return m_idMagazine; }
	QString name() const { return m_name; }
	double priceNetto() const { return m_priceNetto; }
	double priceBrutto() const { return m_priceBrutto; }
	double vat() const { return m_vat; }
	QString jm() const { return m_jm; }
	bool isChecked() const { return m_checked; }
	double detalPrice() const { return m_detalPrice; }
	double marza() const { return m_marza; }
	double count() const { return m_count; }

private:
	int m_id;
	int m_idMagazine;
	QString m_name;
	double m_priceNetto;
	double m_priceBrutto;
	double m_vat;
	QString m_jm;
	double m_detalPrice;
	double m_marza;
	bool m_checked;
	double m_count;
};

#endif //__ARTICLE_RECORD_H__
