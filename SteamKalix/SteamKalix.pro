#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T20:11:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SteamKalix
TEMPLATE = app

SOURCES += main.cpp\
        steamkalix.cpp \
        login.cpp \
        persistentcookiejar.cpp \
        settingsmanager.cpp \
        buy.cpp \
        networkmanager.cpp \
        listing.cpp \
        replytimeout.cpp \
        threadmanager.cpp \
        sell.cpp \
        helper.cpp \
        accountdata.cpp \
        listingsmanager.cpp \
        output.cpp

HEADERS  += steamkalix.h \
        login.h \
        persistentcookiejar.h \
        settingsmanager.h \
        buy.h \
        networkmanager.h \
        listing.h \
        replytimeout.h \
        threadmanager.h \
        sell.h \
        helper.h \
        accountdata.h \
        listingsmanager.h \
        defines.h \
        output.h

FORMS += steamkalix.ui

RESOURCES += resources.qrc

#-------------------------- WINDOWS --------------------------

win32:LIBS +=   -LC:/OpenSSL-Win32/lib -llibeay32 \
                -lssleay32

win32:INCLUDEPATH += C:/OpenSSL-Win32/include

#-------------------------- LINUX --------------------------

unix:LIBS += -lcrypto -lssl
