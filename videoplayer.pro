TEMPLATE = app

QT += webenginewidgets

HEADERS += \
    Utility/DumpUtil.h \
    Utility/IcrCriticalSection.h \
    Utility/ImCharset.h \
    Utility/ImPath.h \
    Utility/LogBuffer.h \
    Utility/LogMacro.h \
    Utility/LogUtil.h \
    mainwindow.h \
    fullscreenwindow.h \
    fullscreennotification.h \
    settingmanager.h

SOURCES += main.cpp \
    Utility/DumpUtil.cpp \
    Utility/IcrCriticalSection.cpp \
    Utility/ImCharset.cpp \
    Utility/ImPath.cpp \
    Utility/LogBuffer.cpp \
    Utility/LogUtil.cpp \
    mainwindow.cpp \
    fullscreenwindow.cpp \
    fullscreennotification.cpp \
    settingmanager.cpp

RESOURCES += \
    data/videoplayer.qrc

RC_ICONS = res/nwlogo.ico

TARGET = niwenplay

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
