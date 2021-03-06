#-------------------------------------------------
#
# Project created by QtCreator 2020-07-23T18:02:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Desktop
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

CONFIG += c++11

SOURCES += \
        main.cpp \
    player/vlcplayer.cpp \
        widget.cpp \
    setting.cpp \
    finddesktop/finddesktop.cpp \
    myclock.cpp \
    Screen/screen.cpp

HEADERS += \
    player/vlcplayer.h \
        widget.h \
    setting.h \
    finddesktop/finddesktop.h \
    myclock.h \
    Screen/screen.h

FORMS += \
        widget.ui \
    setting.ui \
    myclock.ui

win32{
    INCLUDEPATH += $$PWD/sdk/include
    LIBS += -L$$PWD/sdk/lib -llibvlc -llibvlccore
}else{
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lvlc -lvlccore
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
RC_ICONS = DeskGirl.ico
