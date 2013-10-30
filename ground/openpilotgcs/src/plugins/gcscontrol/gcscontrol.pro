TEMPLATE = lib
TARGET = GCSControl 
QT += svg
QT += opengl
QT += network

include(../../openpilotgcsplugin.pri) 
include(../../plugins/coreplugin/coreplugin.pri) 
include(../../plugins/uavobjects/uavobjects.pri)
include(../../libs/sdlgamepad/sdlgamepad.pri)

macx {
    INCLUDEPATH += /Library/Frameworks/SDL.framework/Headers
}
macx {
    SDL = -F/Library/Frameworks
    # Add SDL to CFLAGS fixes build problems on mac
    QMAKE_CFLAGS += $$SDL
    QMAKE_CXXFLAGS += $$SDL
    # Let the linker know where to find the frameworks
    LIBS += $$SDL
}


HEADERS += gcscontrolgadget.h \
    gcscontrolgadgetconfiguration.h \
    gcscontrolgadgetoptionspage.h
HEADERS += joystickcontrol.h
HEADERS += gcscontrolgadgetwidget.h
HEADERS += gcscontrolgadgetfactory.h
HEADERS += gcscontrolplugin.h

SOURCES += gcscontrolgadget.cpp \
    gcscontrolgadgetconfiguration.cpp \
    gcscontrolgadgetoptionspage.cpp
SOURCES += gcscontrolgadgetwidget.cpp
SOURCES += gcscontrolgadgetfactory.cpp
SOURCES += gcscontrolplugin.cpp
SOURCES += joystickcontrol.cpp

OTHER_FILES += GCSControl.pluginspec

FORMS += gcscontrol.ui \
    gcscontrolgadgetoptionspage.ui

RESOURCES += gcscontrol.qrc
