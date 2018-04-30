#ifndef __MDI_AREA_H__
#define __MDI_AREA_H__

#include <qmdiarea.h>

// <summary>
// tworzy nowy lepszy wygląd oraz wyświetla informacje w oknie głównym
// </summary>
class MdiArea : public QMdiArea {
	Q_OBJECT

public:
	MdiArea(QWidget* parent = 0);

public slots:
	// zmienia informację na głównym oknie (zalogowany lub nie)
	void setLocked(bool locked);

protected:
	// zdarzenie malowania obiektów na oknie
	void paintEvent(QPaintEvent* Event) override;

private:
	bool m_locked;
	QPixmap m_pixmap;
};

#endif //__MDI_AREA_H__
