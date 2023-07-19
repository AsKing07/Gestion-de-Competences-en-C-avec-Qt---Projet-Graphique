QT       += core gui sql

QT  += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    competences.cpp \
    dbconnection.cpp \
    gestionEmploye.cpp \
    main.cpp \
    accueil.cpp \
    menu.cpp \
    professionnel.cpp \
    profil.cpp

HEADERS += \
    accueil.h \
    competences.h \
    dbconnection.h \
    gestionEmploye.h \
    menu.h \
    professionnel.h \
    profil.h

FORMS += \
    accueil.ui \
    competences.ui \
    gestionEmploye.ui \
    menu.ui \
    professionnel.ui \
    profil.ui

TRANSLATIONS += \
    Projet_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images/images.qrc

QT_USE_QSTRINGBUILDER = 1
QMAKE_CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8
DEFINES += UNICODE
