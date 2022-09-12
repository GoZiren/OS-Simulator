QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Directory.cpp \
    FCB.cpp \
    calculator.cpp \
    main.cpp \
    mainwindow.cpp \
    memory.cpp \
    process.cpp \
    secdialog.cpp \
    task1.cpp \
    task2.cpp \
    task3.cpp \
    thirdDialog.cpp \
    thread_pool.cpp

HEADERS += \
    AccessControl.h \
    Directory.h \
    FCB.h \
    calculator.h \
    header.h \
    mainwindow.h \
    secdialog.h \
    task1.h \
    task2.h \
    task3.h \
    thirdDialog.h

FORMS += \
    calculator.ui \
    mainwindow.ui \
    secdialog.ui \
    thirdDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    img/images.png \
    img/images_1.png \
    img/login.jpg \
    img/login_1.jpg

RESOURCES += \
    img/resource.qrc
