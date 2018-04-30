#ifndef __MAGAZINE_MANAGER_H__
#define __MAGAZINE_MANAGER_H__

#include <qwidget.h>
#include <qpushbutton.h>
#include <qtreeview.h>

#include "magazineModel.h"

// <summary>
// zarządzanie magazynami artykułów
// </summary>
class MagazineManager : public QWidget {
	Q_OBJECT

public:
	MagazineManager(QWidget* parent = 0);

private:
	void createWidgets();

	QTreeView*		m_treeView;
	QPushButton*	m_addButton;
	QPushButton*	m_removeButton;

	// modele danych
	MagazineModel* m_model;

private slots:
	void addMagazine();	// dodaje nowy magazyn do listy
	void listDoubleClicked(const QModelIndex& index);
};

#endif //MAGAZINE_MANAGER_H__
