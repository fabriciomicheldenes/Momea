QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataTransfer.cpp \
    WriteBufferToFileTask.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DataTransfer.h \
    WriteBufferToFileTask.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    MomeaCapture_pt_BR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += 'C:/Program Files (x86)/Cypress/EZ-USB FX3 SDK/1.3/library/cpp/inc'
DEPENDPATH += 'C:/Program Files (x86)/Cypress/EZ-USB FX3 SDK/1.3/library/cpp/inc'

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/Cypress/EZ-USB FX3 SDK/1.3/library/cpp/lib/x64/' -lCyAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/Cypress/EZ-USB FX3 SDK/1.3/library/cpp/lib/x64/' -lCyAPI

LIBS += -luser32 \
        -lsetupapi \
        -llegacy_stdio_definitions \

# win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/Cypress/EZ-USB FX3 SDK/1.3/library/cpp/lib/x64/' -lCyAPI
# else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/Cypress/EZ-USB FX3 SDK/1.3/library/cpp/lib/x64/' -lCyAPI

# INCLUDEPATH += 'C:/Program Files (x86)/Cypress/EZ-USB FX3 SDK/1.3/library/cpp/inc'
# DEPENDPATH += 'C:/Program Files (x86)/Cypress/EZ-USB FX3 SDK/1.3/library/cpp/inc'
