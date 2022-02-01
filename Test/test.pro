TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L../Compiler/release -lcompiler
LIBS += -L../../unit/release -lunit

PRE_TARGETDEPS += ../Compiler/release/libcompiler.a

INCLUDEPATH += \
    ../Compiler \
    ../../unit

SOURCES += \
    main.cpp

HEADERS +=
