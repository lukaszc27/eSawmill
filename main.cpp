#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include <qfile.h>
#include <qsettings.h>
#include <qstylefactory.h>

#include "mainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Utf-8"));

	

	a.setWindowIcon(QIcon("icons/mainIcon.png"));
	a.setOrganizationName("TPUH");
	a.setOrganizationDomain("localhost");
	a.setApplicationName("eSawmill");
	a.setApplicationVersion("1.0 (ALFA)");
	a.setApplicationDisplayName(QString("%1 ver.%2")
		.arg(QApplication::applicationName())
		.arg(QApplication::applicationVersion()));

	QSettings settings;
	QPalette darkPalette;
	switch (settings.value("app/style").toInt())
	{
	case 1:
		a.setStyle(QStyleFactory::create("Fusion"));
		break;

	case 2:
		a.setStyle(QStyleFactory::create("Fusion"));

		darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
		darkPalette.setColor(QPalette::WindowText, Qt::white);
		darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
		darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
		darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
		darkPalette.setColor(QPalette::ToolTipText, Qt::white);
		darkPalette.setColor(QPalette::Text, Qt::white);
		darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
		darkPalette.setColor(QPalette::ButtonText, Qt::white);
		darkPalette.setColor(QPalette::BrightText, Qt::red);
		darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

		darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
		darkPalette.setColor(QPalette::HighlightedText, Qt::black);

		a.setPalette(darkPalette);
		break;

	default:
		break;
	}

	MainWindow mainWindow;
	mainWindow.showMaximized();

	return a.exec();
}
