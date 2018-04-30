#ifndef __MY_TREE_VIEW_H__
#define __MY_TREE_VIEW_H__

#include <qtreeview.h>

// <summary>
// nowa udoskonalona klasa wyświetlania modeli
// </summary>
class MyTreeView : public QTreeView {
	Q_OBJECT

public:
	MyTreeView(QWidget* parent = 0);

protected:
	void keyPressEvent(QKeyEvent* Event) override;
};

#endif //__MY_TREE_VIEW_H__
