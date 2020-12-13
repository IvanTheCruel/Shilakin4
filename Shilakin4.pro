TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    pipe.cpp \
    station.cpp \
    check_input.cpp \
    mymenu.cpp \
    web.cpp

HEADERS += \
    pipe.h \
    station.h \
    check_input.h \
    mymenu.h \
    web.h
