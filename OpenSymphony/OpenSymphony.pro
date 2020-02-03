#-------------------------------------------------
#
# Project created by QtCreator 2020-01-20T16:22:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenSymphony
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    track.cpp \
    project.cpp

HEADERS += \
        mainwindow.h \
    track.h \
    project.h

FORMS += \
        mainwindow.ui \
    newtrack.ui

LIBS += -L"D:/Program Files/MATLAB/R2019a/extern/lib/win64/microsoft" -llibeng -llibmx

LIBS += -L"C:/Program Files/MATLAB/R2019a/extern/lib/win64/microsoft" \
           -llibeng \
           -llibmx

INCLUDEPATH += "D:/Program Files/MATLAB/R2019a/extern/include"
INCLUDEPATH += "C:/Program Files/MATLAB/R2019a/extern/include"
