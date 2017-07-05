QT += qml quick multimedia charts

CONFIG += c++11
TARGET = r1bcx
SOURCES += main.cpp \
    c_audio.cpp \
    c_audio_input.cpp \
    xyseriesiodevice.cpp \
    spectrum/frequencyspectrum.cpp \
    spectrum/spectrumanalyser.cpp \
    spectrum/utils.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


HEADERS += \
    c_audio.h \
    c_audio_input.h \
    xyseriesiodevice.h \
    spectrum/frequencyspectrum.h \
    spectrum/spectrum.h \
    spectrum/utils.h \
    spectrum/spectrumanalyser.h \
    spectrum/utils.h \




INCLUDEPATH += spectrum\
INCLUDEPATH += ../3rdparty/fftreal\

DISTFILES += \
    js/Log.js \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat
win32 {
    #message("* Using settings for WINDOWS->LIBRARY")
    #message($$OUT_PWD)
    LIBS += -L$$OUT_PWD -llibfftreal
}
android {
    ANDROID_PACKAGE_SOURCE_DIR=$$_PRO_FILE_PWD_/android
    QT += androidextras
    #message("* Using settings for Android-->LIBRARY")
    #message("-L$$OUT_PWD -llibfftreal")
    LIBS += "-L$$OUT_PWD libfftreal.so"
    #LIBS += -L$$OUT_PWD/../3rdparty/fftreal -llibfftreal
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
