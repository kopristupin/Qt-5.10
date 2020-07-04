QT       += core gui sql
QT +=charts
QT+= printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    archive.cpp \
    callout.cpp \
    chart.cpp \
    checking.cpp \
    customlabel.cpp \
    dialog.cpp \
    fuelconsumption.cpp \
    labeltochangetext.cpp \
    listofparts.cpp \
    main.cpp \
    mainwindow.cpp \
    makeentry.cpp \
    notdoneworksc.cpp \
    order.cpp

HEADERS += \
    archive.h \
    callout.h \
    chart.h \
    chart_копия.h \
    checking.h \
    customlabel.h \
    dialog.h \
    fuelconsumption.h \
    labeltochangetext.h \
    listofparts.h \
    mainwindow.h \
    makeentry.h \
    notdoneworksc.h \
    order.h \
Printer.h

FORMS += \
    archive.ui \
    chart.ui \
    checking.ui \
    dialog.ui \
    fuelconsumption.ui \
    listofparts.ui \
    mainwindow.ui \
    makeentry.ui \
    notdoneworksc.ui \
    order.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    prefix1.qrc
