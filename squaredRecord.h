#ifndef __SQUARED_RECORD_H__
#define __SQUARED_RECORD_H__

#include <qobject.h>

// <summary>
// reprezentacja pojedynczego elementu konstrukcyjnego
// </summary>
class SquaredRecord
{
public:
	SquaredRecord(float width, float height, float length, float count, bool planed = false, bool checked = false)
	{
		setWidth(width);
		setHeight(height);
		setLength(length);
		setCount(count);
		setChecked(checked);
		setPlaned(planed);
	}
	SquaredRecord() {};

	void setWidth(float width) { m_width = width; }
	void setHeight(float height) { m_height = height; }
	void setLength(float length) { m_length = length; }
	void setCount(float count) { m_count = count; }
	void setChecked(bool checked) { m_checked = checked; }
	void setPlaned(bool planed) { m_planed = planed; }

	float width() const { return m_width; }
	float height() const { return m_height; }
	float length() const { return m_length; }
	float count() const { return m_count; }
	bool isChecked() const { return m_checked; }
	bool isPlaned() const { return m_planed; }

	/**
	 * @brief metric
	 * @return Ilość m3
	 */
	double metric() {
		double w = m_width / 100; // cm -> m
		double h = m_height / 100;

		return (w * h * m_length) * m_count;
	}

private:
	bool m_checked;
	bool m_planed;
	float m_width;
	float m_height;
	float m_length;
	float m_count;
};

#endif //__SQUARED_RECORD_H__
