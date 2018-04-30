#ifndef __MAGAZINE_RECORD_H__
#define __MAGAZINE_RECORD_H__

#include <qobject.h>
#include <qstring.h>

class MagazineRecord
{
public:
	MagazineRecord(const QString& name,
		const QString& description,
		bool checked = false,
		int id = 0)
	{
		m_name = name;
		m_description = description;
		m_checked = checked;
		m_id = id;
	}

	void setName(const QString& name) { m_name = name; }
	void setDescription(const QString& descrpition) { m_description = descrpition; }
	void setChecked(bool checked) { m_checked = checked; }
	void setId(int id) { m_id = id; }

	const QString& name() const { return m_name; }
	const QString& description() const { return m_description; }
	bool isChecked() const { return m_checked; }
	int id() const { return m_id; }

private:
	QString m_name;
	QString m_description;
	bool m_checked;
	int m_id;
};

#endif //__MAGAZINE_RECORD_H__
