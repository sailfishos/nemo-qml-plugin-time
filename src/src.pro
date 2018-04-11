TARGET = nemotime
PLUGIN_IMPORT_PATH = Nemo/Time

TEMPLATE = lib
CONFIG += qt plugin hide_symbols c++11
QT = qml

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmldir.files += $$_PRO_FILE_PWD_/qmldir
qmldir.path +=  $$target.path
INSTALLS += qmldir

SOURCES += plugin.cpp \
           nemowallclock.cpp

HEADERS += nemowallclock.h \
            nemowallclock_p.h

LIBS += -ltimed-qt5

QT += dbus
SOURCES += nemowallclock_meego.cpp
#    SOURCES += nemowallclock_generic.cpp
