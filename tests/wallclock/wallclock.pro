include(../common.pri)
TARGET = tst_wallclock

SOURCES += tst_wallclock.cpp

tests_qml.target = update.qml
tests_qml.files = update.qml
tests_qml.path = /opt/tests/nemo-qml-plugin-time-qt$${QT_MAJOR_VERSION}

DEFINES += TEST_DIRECTORY=\\\"$$tests_qml.path\\\"
INSTALLS += tests_qml

