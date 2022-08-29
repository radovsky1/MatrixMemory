QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bcrypt.cpp \
    blowfish.cpp \
    card.cpp \
    main.cpp \
    mainwindow.cpp \
    matrixwidget.cpp \
    plotwidget.cpp \
    statswindow.cpp \
    user.cpp

HEADERS += \
    bcrypt.h \
    card.h \
    mainwindow.h \
    matrixwidget.h \
    node_blf.h \
    openbsd.h \
    plotwidget.h \
    statswindow.h \
    user.h

FORMS += \
    card.ui \
    mainwindow.ui \
    matrixwidget.ui \
    plotwidget.ui \
    statswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../build-MatrixMemory-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/resources/iconq.PNG
