TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -g -lSDL2 -lGL  -lGLU -lglut

SOURCES += \
    main.cpp \
    robotopengl.cpp

HEADERS += \
    robotopengl.hpp

