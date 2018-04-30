#-------------------------------------------------
#
# Project created by QtCreator 2018-04-28T08:25:42
#
#-------------------------------------------------

QT       += core gui network sql xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eSawmill
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    accessDialog.cpp \
    addArticleDialog.cpp \
    addContrahentDialog.cpp \
    addGroupSquaredDialog.cpp \
    addMagazineDialog.cpp \
    addOrderDialog.cpp \
    addRoundDialog.cpp \
    addServiceDialog.cpp \
    addSingleSquaredDialog.cpp \
    articleDelegate.cpp \
    articleManagerModel.cpp \
    articleModel.cpp \
    articlesManager.cpp \
    clientDocument.cpp \
    configurationDialog.cpp \
    contrahentDelegate.cpp \
    contrahentManager.cpp \
    contrahentModel.cpp \
    contrahentWidget.cpp \
    configurationWidgets.cpp\
    dateFilterDialog.cpp \
    elementsFilter.cpp \
    emailaddress.cpp \
    groupSquaredDelegate.cpp \
    groupSquaredModel.cpp \
    magazineManager.cpp \
    magazineModel.cpp \
    main.cpp \
    mainWindow.cpp \
    mdiArea.cpp \
    mimeattachment.cpp \
    mimecontentformatter.cpp \
    mimefile.cpp \
    mimehtml.cpp \
    mimeinlinefile.cpp \
    mimemessage.cpp \
    mimemultipart.cpp \
    mimepart.cpp \
    mimetext.cpp \
    myTreeView.cpp \
    ordersAnaliseAndOptimize.cpp \
    ordersManager.cpp \
    ordersModel.cpp \
    ordersProxyModel.cpp \
    quotedprintable.cpp \
    realizedOrdersFilter.cpp \
    roundDelegate.cpp \
    roundModel.cpp \
    saleWidget.cpp \
    serviceManager.cpp \
    serviceModel.cpp \
    smtpclient.cpp \
    squaredDelegate.cpp \
    squaredModel.cpp

HEADERS += \
    accessDialog.h \
    addArticleDialog.h \
    addContrahentDialog.h \
    addGroupSquaredDialog.h \
    addMagazineDialog.h \
    addOrderDialog.h \
    addServiceDialog.h \
    addSingleSquaredDialog.h \
    articleDelegate.h \
    articleManagerModel.h \
    articleModel.h \
    articleRecord.h \
    articlesManager.h \
    clientDocument.h \
    configurationDialog.h \
    configurationWidgets.h \
    contrahentDelegate.h \
    contrahentManager.h \
    contrahentModel.h \
    contrahentRecord.h \
    contrahentWidget.h \
    dateFilterDialog.h \
    emailaddress.h \
    groupSquaredDelegate.h \
    groupSquaredModel.h \
    magazineManager.h \
    magazineModel.h \
    magazineRecord.h \
    mainWindow.h \
    mdiArea.h \
    mimeattachment.h \
    mimecontentformatter.h \
    mimefile.h \
    mimehtml.h \
    mimeinlinefile.h \
    mimemessage.h \
    mimemultipart.h \
    mimepart.h \
    mimetext.h \
    myTreeView.h \
    orderRecord.h \
    ordersAnaliseAndOptimize.h \
    ordersManager.h \
    ordersModel.h \
    ordersProxyModel.h \
    quotedprintable.h \
    realizedOrdersFilter.h \
    roundDelegate.h \
    roundModel.h \
    roundRecord.h \
    saleWidget.h \
    serviceManager.h \
    serviceModel.h \
    serviceRecord.h \
    smtpclient.h \
    smtpexports.h \
    SmtpMime.h \
    squaredDelegate.h \
    squaredModel.h \
    squaredRecord.h \
    elementsFilter.h \
    addRoundDialog.h \
    configurationWidgets.h

SUBDIRS += \
    eSawmill.pro

DISTFILES +=
