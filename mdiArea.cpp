#include "mdiArea.h"
#include <qpainter.h>
#include <qevent.h>
#include <qapplication.h>
#include <qmessagebox.h>

MdiArea::MdiArea(QWidget* parent)
	: QMdiArea(parent)
	, m_locked(true)
{
	if (m_locked)
		if (!m_pixmap.load("icons/lock.png"))
		{
			QMessageBox::warning(parent, tr("Błąd"),
				tr("Nie załadowano ikony cancel.png"));

			return;
		}
}

//-------------------------------------------
// wyświetla odpowiedni komunikat w oknie
// głónym aplikacji
void MdiArea::setLocked(bool locked)
{
	m_locked = locked;
	update();
}

//-------------------------------------------
// zdarzenie malowania widgetów oraz elementów
void MdiArea::paintEvent(QPaintEvent * Event)
{
	QMdiArea::paintEvent(Event);

	QPainter painter(viewport());

	// wyświetlenie inforamcji czy zalogowanie do programu się powiodło
	if (m_locked)
	{
		QString information(tr("Nie zalogowano do bazy daych programu %1").arg(QApplication::applicationName()));
		
		int x = width() / 2 - information.length()*18 / 2;
		int y = height() / 2 - information.size() / 2 - 40;
		
		painter.setFont(QFont("Arial black", 24));
		painter.setPen(Qt::white);
		painter.drawText(x, y, information);
		painter.drawPixmap(width() / 2 - m_pixmap.width() / 2, height() / 2 - 30, m_pixmap);
	}

	// wyświetlanie informacji o wersji debug
#if _DEBUG
	int x, y;
	x = width() - 300;
	y = height() - 75;

	painter.setFont(QFont("Arial", 16));
	painter.setPen(Qt::white);
	painter.drawText(x, y, tr("DEBUG"));
	painter.drawText(x, y+painter.font().pointSize()+3, tr("Kompilacja: %1").arg(__DATE__));
	painter.drawText(x, y + 2*(painter.font().pointSize() + 3), tr("Godzina: %1").arg(__TIME__));
#endif
}
