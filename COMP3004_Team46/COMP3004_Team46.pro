QT       += core gui
QT        += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

source_dir = src
forms_dir = ui
resources_dir = res

SOURCES += \
    $${source_dir}/main.cpp \
    $${source_dir}/mainwindow.cpp \
    src/battery.cpp \
    src/dbmanager.cpp \
    src/log.cpp \
    src/menu.cpp \
    src/session.cpp

HEADERS += \
    $${source_dir}/mainwindow.h \
    src/battery.h \
    src/dbmanager.h \
    src/log.h \
    src/menu.h \
    src/session.h

FORMS += \
    $${forms_dir}/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    $${resources_dir}/Resources.qrc
