TARGET = nemotime
PLUGIN_IMPORT_PATH = Nemo/Time

TEMPLATE = lib
CONFIG += qt plugin hide_symbols c++11
QT = qml

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmldir.files += qmldir plugins.qmltypes
qmldir.path +=  $$target.path
INSTALLS += qmldir

qmltypes.commands = qmlplugindump -nonrelocatable Nemo.Time 1.0 > $$PWD/plugins.qmltypes
QMAKE_EXTRA_TARGETS += qmltypes

SOURCES += plugin.cpp \
           nemowallclock.cpp

HEADERS += nemowallclock.h \
            nemowallclock_p.h

LIBS += -ltimed-qt5

QT += dbus
SOURCES += nemowallclock_meego.cpp
#    SOURCES += nemowallclock_generic.cpp
