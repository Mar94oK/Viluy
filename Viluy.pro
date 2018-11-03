#-------------------------------------------------
#
# Project created by QtCreator 2018-02-16T09:07:45
#
#-------------------------------------------------

QT       += core gui
QT       += network

#CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Viluy
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
        gamesettings.cpp \
        card.cpp \
        server.cpp \
        room.cpp \
        serversettings.cpp \
        utilites.cpp \
        serverMessageSystem.pb.cc

HEADERS  += mainwindow.h \
        gamesettings.h \
        card.h \
        cardsborderdefines.h \
        server.h \
        room.h \
        serversettings.h \
        utilites.h \
        serverMessageSystem.pb.h

FORMS    += mainwindow.ui

#INCLUDEPATH += "D:\vpkg\vcpkg\installed\x64-windows\include"
#INCLUDEPATH += "D:\vpkg\vcpkg\installed\x64-windows\lib"
#INCLUDEPATH += "D:\vpkg\vcpkg\installed\x64-windows"
#LIBS += -lprotobuf
#LIBS += -L"D:\vpkg\vcpkg\installed\x64-windows\lib" -llibprotobuf
#LIBS += -L
#LIBS += -L-lprotobuf
#INCLUDEPATH += "/usr/local/lib/"
#LIBS += -lprotobuf


unix: LIBS += /usr/local/lib/libprotobuf.a
#else:win32: LIBS += D:\TheNewestProtobuf\protobuf-3.6.1\src\.libs\libprotobuf.a
#win32: INCLUDEPATH += "D:\TheNewestProtobuf\protobuf-3.6.1\src"


#win32: INCLUDEPATH += "D:\Protobuf\protobuf-cpp-3.6.1\protobuf-3.6.1\src\.libs"
#win32: DEPENDPATH += "D:\Protobuf\protobuf-cpp-3.6.1\protobuf-3.6.1\src\.libs"

#win32: PRE_TARGETDEPS += "D:\Protobuf\protobuf-cpp-3.6.1\protobuf-3.6.1\src\.libs\libprotoc.a"

#else:win32: LIBS += libprotobuf.a
#else:win32: LIBS += C:\Qt\5.8\mingw53_32\lib\libprotobuf.a
#else:win32: LIBS += -lprotobuf
#win32: INCLUDEPATH += D:\Protobuf\protobuf-cpp-3.6.1\protobuf-3.6.1\src
#win32: INCLUDEPATH += D:\Protobuf\protobuf-cpp-3.6.1\protobuf-3.6.1\src\.libs
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lprotobuf
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lprotobuf
#else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lprotobuf

#INCLUDEPATH += $$PWD/../../../../usr/local/include
#DEPENDPATH += $$PWD/../../../../usr/local/include

DISTFILES += \
    ToDoList \
    serverMessageSystem.proto \
    ProtobufWin32BuildNotes.txt



#win32: LIBS += -llibprotobuf
#win32:INCLUDEPATH += "..\\...\\directoty_protobuf\\src"
#INCLUDEPATH += "D:\\Protobuf\\protobuf-cpp-3.6.1\\protobuf-3.6.1\\src"
#INCLUDEPATH += $$PWD/../../Protobuf/protobuf-cpp-3.6.1/protobuf-3.6.1/src/.libs
#DEPENDPATH += $$PWD/../../Protobuf/protobuf-cpp-3.6.1/protobuf-3.6.1/src/.libs

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../Protobuf/protobuf-cpp-3.6.1/protobuf-3.6.1/src/.libs/protobuf.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/../../Protobuf/protobuf-cpp-3.6.1/protobuf-3.6.1/src/.libs/libprotobuf.a



win32: LIBS += $$PWD/Libs/libprotobuf.a

INCLUDEPATH += $$PWD/Includes
DEPENDPATH += $$PWD/Includes

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/Libs/protobuf.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/Libs/libprotobuf.a
