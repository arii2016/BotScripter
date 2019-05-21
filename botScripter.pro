#-------------------------------------------------
#
# Project created by QtCreator 2018-08-27T09:13:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = botScripter
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

VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_BUILD = 3

DEFINES += APP_VERSION_MAJOR=$$VERSION_MAJOR \
       APP_VERSION_MINOR=$$VERSION_MINOR \
       APP_VERSION_BUILD=$$VERSION_BUILD

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
QMAKE_TARGET_COMPANY =
QMAKE_TARGET_PRODUCT = Conversation Scripter
QMAKE_TARGET_DESCRIPTION =
QMAKE_TARGET_COPYRIGHT = (C)2018


INCLUDEPATH += src

SOURCES +=                              \
        src/main.cpp                    \
        src/ui/mainWindow.cpp           \
        src/ui/scriptNameSetting.cpp    \
        src/data/dataManager.cpp        \
        src/data/scriptEntity.cpp

HEADERS +=                              \
        src/ui/mainWindow.h             \
        src/ui/scriptNameSetting.h      \
        src/data/dataManager.h          \
        src/data/scriptEntity.h         \
        src/utility.h

FORMS +=                                \
        src/ui/mainWindow.ui            \
        src/ui/scriptNameSetting.ui

RESOURCES += \
    rc.qrc

RC_ICONS = scripter.ico

DISTFILES += fabool_fomat.astylerc
