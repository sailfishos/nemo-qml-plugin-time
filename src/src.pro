TARGET = nemotime
PLUGIN_IMPORT_PATH = org/nemomobile/time

TEMPLATE = lib
CONFIG += qt plugin hide_symbols
equals(QT_MAJOR_VERSION, 4): QT += declarative
equals(QT_MAJOR_VERSION, 5): QT += qml

equals(QT_MAJOR_VERSION, 4): target.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH
equals(QT_MAJOR_VERSION, 5): target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmldir.files += $$_PRO_FILE_PWD_/qmldir
qmldir.path +=  $$target.path
INSTALLS += qmldir

SOURCES += plugin.cpp \
           nemowallclock.cpp

HEADERS += nemowallclock.h \
            nemowallclock_p.h

equals(QT_MAJOR_VERSION, 4): LIBS += -ltimed
equals(QT_MAJOR_VERSION, 5): LIBS += -ltimed-qt5

QT += dbus
SOURCES += nemowallclock_meego.cpp
#    SOURCES += nemowallclock_generic.cpp
