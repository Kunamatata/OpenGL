TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lSDL2 -lGL -lGLU -lGLEW -lassimp -lglut

SOURCES += main.cpp \
    light.cpp

HEADERS += \
    light.hpp
