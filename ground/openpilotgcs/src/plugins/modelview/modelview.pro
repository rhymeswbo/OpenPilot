TEMPLATE = lib
TARGET = ModelViewGadget
include(../../openpilotgcsplugin.pri)
include(../../plugins/coreplugin/coreplugin.pri)
include(../../libs/glc_lib/glc_lib.pri)
include(modelview_dependencies.pri)
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
INCLUDEPATH += ../../libs/glc_lib
HEADERS += modelviewplugin.h \
    modelviewgadgetconfiguration.h \
    modelviewgadget.h \
    modelviewgadgetwidget.h \
    modelviewgadgetfactory.h \
    modelviewgadgetoptionspage.h
SOURCES += modelviewplugin.cpp \
    modelviewgadgetconfiguration.cpp \
    modelviewgadget.cpp \
    modelviewgadgetfactory.cpp \
    modelviewgadgetwidget.cpp \
    modelviewgadgetoptionspage.cpp
OTHER_FILES += ModelViewGadget.pluginspec
FORMS += modelviewoptionspage.ui

RESOURCES += \
    modelview.qrc
