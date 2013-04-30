include(../common.pri)
TARGET = tst_wallclock

SOURCES += tst_wallclock.cpp

tests_qml.target = update.qml
tests_qml.files = update.qml
equals(QT_MAJOR_VERSION, 4): tests_qml.path = /opt/tests/nemo-qml-plugins/time
equals(QT_MAJOR_VERSION, 5): tests_qml.path = /opt/tests/nemo-qml-plugins-qt5/time

DEFINES += TEST_DIRECTORY=\\\"$$tests_qml.path\\\"
INSTALLS += tests_qml

