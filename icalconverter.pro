TARGET = icalconverter

CONFIG += link_pkgconfig
PKGCONFIG += libkcalcoren-qt5 libmkcal-qt5

SOURCES += src/icalconverter.cpp

OTHER_FILES += \
    rpm/icalconverter.spec \
    rpm/icalconverter.yaml

target.path = $$INSTALL_ROOT/usr/bin/
INSTALLS += target
