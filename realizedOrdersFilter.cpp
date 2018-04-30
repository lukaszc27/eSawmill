#include "realizedOrdersFilter.h"
#include <qmessagebox.h>


RealizedOrdersFilter::RealizedOrdersFilter(QObject* parent)
	: QSortFilterProxyModel(parent)
{
	m_db = QSqlDatabase::database();
	m_filterEnable = false;
}

//-------------------------------------------------
// aktywuje lub dezaktywuje filter zamówień
// zrealizowanych
void RealizedOrdersFilter::setFilterEnable(int state)
{
	if (state)
		m_filterEnable = true;
	else m_filterEnable = false;

	invalidateFilter();
}

//-------------------------------------------------
// odfiltorwuje zamówienia zrealizowane 
// od niezrealizowanych
bool RealizedOrdersFilter::filterAcceptsRow(int sourceRow, const QModelIndex & sourceParent) const
{
	if (!m_filterEnable)
	{
		if (m_db.isOpen() && m_db.isValid())
		{
			QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
			QVariant value = index.data(Qt::UserRole);

			QSqlQuery q;
			q.prepare("SELECT zrealizowane FROM zamowienia WHERE zamowienieId = ?");
			q.bindValue(0, value.toInt());

			if (q.exec())
			{
				if (q.first())
				{
					if (q.value(0).toBool())
						return false;

					return true;
				}
			}
			else
			{
				QMessageBox::critical(0, tr("Błąd"),
					q.lastError().text());

				return false;
			}
		}
	}

	return true;
}
