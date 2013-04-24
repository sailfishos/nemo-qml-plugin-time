QT += testlib
equals(QT_MAJOR_VERSION, 4): QT += declarative
equals(QT_MAJOR_VERSION, 5): QT += qml
TEMPLATE = app

equals(QT_MAJOR_VERSION, 4): target.path = /opt/tests/nemo-qml-plugins/time
equals(QT_MAJOR_VERSION, 5): target.path = /opt/tests/nemo-qml-plugins-qt5/time
INSTALLS += target
