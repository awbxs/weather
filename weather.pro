QT       += core gui network  texttospeech  multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
#    ../../Speech Recognition/BaiDu_Speech/audio.cpp \
#    ../../Speech Recognition/BaiDu_Speech/http.cpp \
#    ../../Speech Recognition/BaiDu_Speech/speech.cpp
    audio.cpp \
    http.cpp \
    speech.cpp

HEADERS += \
    mainwindow.h \
    weatherday.h \
    weathertool.h \
#    ../../Speech Recognition/BaiDu_Speech/audio.h \
#    ../../Speech Recognition/BaiDu_Speech/http.h \
#    ../../Speech Recognition/BaiDu_Speech/speech.h
    audio.h \
    http.h \
    speech.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += main.qrc
