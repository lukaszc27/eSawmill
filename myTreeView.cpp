#include "myTreeView.h"
#include <qevent.h>
#include <qmessagebox.h>


MyTreeView::MyTreeView(QWidget* parent)
	: QTreeView(parent)
{
}

//----------------------------------------------
// obsługa klinkięcych klawiszy w QTreeView
void MyTreeView::keyPressEvent(QKeyEvent* Event)
{
	if (Event->key() == Qt::Key_Return)
		emit doubleClicked(currentIndex());

	QTreeView::keyPressEvent(Event);
}
