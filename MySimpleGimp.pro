QT      += core gui widgets

TARGET   = MySimpleGimp
TEMPLATE = app

INCLUDEPATH += src/

SOURCES += src/main.cpp

include(src/core/core.pri)
include(src/gui/gui.pri)

RESOURCES += res/icons.qrc

gcc:QMAKE_CXXFLAGS  += -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder

msvc {
    QMAKE_CXXFLAGS_WARN_ON -= -W3
    QMAKE_CXXFLAGS_WARN_ON += -W2
}

CONFIG += c++11
