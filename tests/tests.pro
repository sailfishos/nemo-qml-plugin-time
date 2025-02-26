TEMPLATE = subdirs
SUBDIRS = wallclock

tests_xml.target = tests.xml
tests_xml.files = tests.xml
tests_xml.path = /opt/tests/nemo-qml-plugin-time-qt$${QT_MAJOR_VERSION}
tests_xml.CONFIG = no_check_exist
tests_xml.commands = sed \'s%@PATH@%$${tests_xml.path}%\' $$PWD/tests.xml.in > $$PWD/tests.xml

QMAKE_DISTCLEAN += tests.xml

INSTALLS += tests_xml
