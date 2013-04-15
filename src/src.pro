TARGET = nemotime
PLUGIN_IMPORT_PATH = org/nemomobile/time

TEMPLATE = lib
CONFIG += qt plugin hide_symbols
QT += declarative

target.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmldir.files += $$_PRO_FILE_PWD_/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$$$PLUGIN_IMPORT_PATH
INSTALLS += qmldir

SOURCES += plugin.cpp \
           nemowallclock.cpp

HEADERS += nemowallclock.h \
            nemowallclock_p.h

LIBS += -ltimed

QT += dbus
SOURCES += nemowallclock_meego.cpp
#    SOURCES += nemowallclock_generic.cpp
