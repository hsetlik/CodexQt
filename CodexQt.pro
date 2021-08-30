QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cardstructure.cpp \
    cardwidget.cpp \
    datastructures.cpp \
    decklistwidget.cpp \
    deckwidget.cpp \
    inputwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    masterstackedwidget.cpp \
    phraseinputform.cpp \
    stlutility.cpp

HEADERS += \
    cardstructure.h \
    cardwidget.h \
    datastructures.h \
    decklistwidget.h \
    deckwidget.h \
    inputwidget.h \
    mainwindow.h \
    masterstackedwidget.h \
    phraseinputform.h \
    stlutility.h

FORMS += \
    cardwidget.ui \
    decklistwidget.ui \
    deckwidget.ui \
    inputform.ui \
    mainwindow.ui \
    phraseinputform.ui

TRANSLATIONS += \
    CodexQt_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
