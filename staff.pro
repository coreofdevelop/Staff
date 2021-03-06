QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.0.1
QMAKE_TARGET_COMPANY = CoreOfDevelop
QMAKE_TARGET_PRODUCT = Staff Portable
QMAKE_TARGET_DESCRIPTION = Program for monitoring employee events
QMAKE_TARGET_COPYRIGHT = Nikolay Duhovskoy
TARGET = Staff

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
    database.cpp \
    editform.cpp \
    filterproxymodel.cpp \
    main.cpp \
    mainwindow.cpp \
    setform.cpp

HEADERS += \
    database.h \
    editform.h \
    filterproxymodel.h \
    mainwindow.h \
    setform.h

FORMS += \
    editform.ui \
    mainwindow.ui \
    setform.ui

RC_ICONS = icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
