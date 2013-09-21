QT += xml
TEMPLATE = lib 
TARGET = MonitorGadget

include(../../openpilotgcsplugin.pri)
include(../../plugins/coreplugin/coreplugin.pri) 
include(../../libs/opmapcontrol/opmapcontrol.pri)
include(../../plugins/uavobjects/uavobjects.pri)
include(../../plugins/uavobjectutil/uavobjectutil.pri)
include(../../plugins/uavtalk/uavtalk.pri)
include(../../libs/utils/utils.pri)

HEADERS += monitorplugin.h
HEADERS += monitorgadget.h
HEADERS += monitorgadgetwidget.h
HEADERS += monitorconfiguration.h
HEADERS += monitoroptionspage.h
HEADERS += monitorgadgetfactory.h
HEADERS += monitorcontentprovider.h
HEADERS += monitorcontent.h
HEADERS += mapcontent/mapcontent.h
HEADERS += mapcontent/mapconfiguration.h
HEADERS += mapcontent/mapoptionspage.h
HEADERS += gpscontent/gpscontent.h
HEADERS += gpscontent/gpscontentwidget.h
HEADERS += cameracontent/cameracontent.h
HEADERS += cameracontent/cameracontentwidget.h
HEADERS += cameracontent/cameraoptionspage.h
HEADERS += cameracontent/cameraconfiguration.h
HEADERS += emptycontent/emptycontent.h
HEADERS += emptycontent/emptycontentwidget.h

SOURCES += monitorplugin.cpp
SOURCES += monitorgadget.cpp
SOURCES += monitorgadgetwidget.cpp
SOURCES += monitorconfiguration.cpp
SOURCES += monitoroptionspage.cpp
SOURCES += monitorgadgetfactory.cpp
SOURCES += monitorcontentprovider.cpp
SOURCES += monitorcontent.cpp
SOURCES += mapcontent/mapcontent.cpp
SOURCES += mapcontent/mapconfiguration.cpp
SOURCES += mapcontent/mapoptionspage.cpp
SOURCES += gpscontent/gpscontent.cpp
SOURCES += gpscontent/gpscontentwidget.cpp
SOURCES += cameracontent/cameracontent.cpp
SOURCES += cameracontent/cameracontentwidget.cpp
SOURCES += cameracontent/cameraoptionspage.cpp
SOURCES += cameracontent/cameraconfiguration.cpp
SOURCES += emptycontent/emptycontent.cpp
SOURCES += emptycontent/emptycontentwidget.cpp

OTHER_FILES += MonitorGadget.pluginspec

FORMS += monitorgadgetwidget.ui \
    monitoroptionspage.ui \
    cameracontent/cameraoptionspage.ui \
    gpscontent/gpsoptionspage.ui \
    mapcontent/mapoptionspage.ui
    