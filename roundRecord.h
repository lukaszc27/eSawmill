#ifndef __ROUND_RECORD_H__
#define __ROUND_RECORD_H__

#include <qobject.h>

// <summary>
// pojedynczy rekord dla listy k³ód w modelu
// </summary>
class RoundRecord
{
public:
	RoundRecord() { };
	RoundRecord(double diameter, double length, bool eaves = false, bool checked = false)
	{
		setDiameter(diameter);
		setLength(length);
		setEaves(eaves);
		setChecked(checked);
	}

	void setDiameter(double diameter) { m_diameter = diameter; }
	void setLength(double length) { m_length = length; }
	void setChecked(bool checked) { m_checked = checked; }
	void setEaves(bool second) { m_second = second; }

	double diameter() const { return m_diameter; }
	double length() const { return m_length; }
	bool isChecked() const { return m_checked; }
	bool isEaves() const { return m_second; }

	double metric() {
		const double Pi = 3.1415;
		double d = diameter()/100; // cm -> m
		double r = d/2;
		double podstawa = Pi*(r*r);

		return podstawa*length();
	}
private:
	double m_diameter;	// œrednica
	double m_length;	// d³ugoœæ
	bool m_checked;		// czy zaznaczony
	bool m_second;		// czy k³oda jest ciêta podwójnie (kantowana)
};

#endif //__ROUND_RECORD_H__
