QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AndroidGui/androidslider.cpp \
    AndroidGui/androidslider_shoot.cpp \
    aabb.cpp \
    boar.cpp \
    bullet.cpp \
    enemy.cpp \
    gamewidget.cpp \
    global.cpp \
    goblin.cpp \
    main.cpp \
    obstacle.cpp \
    passageway.cpp \
    player.cpp \
    room.cpp \
    scene.cpp \
    startwidget.cpp \
    weapon.cpp

HEADERS += \
    AndroidGui/androidslider.h \
    AndroidGui/androidslider_shoot.h \
    Entity.h \
    aabb.h \
    boar.h \
    bullet.h \
    enemy.h \
    gamewidget.h \
    global.h \
    goblin.h \
    obstacle.h \
    passageway.h \
    player.h \
    room.h \
    scene.h \
    startwidget.h \
    weapon.h

FORMS += \
    gamewidget.ui \
    startwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc
