win32|macx:TARGET = TextSeeker
else:TARGET = textseeker

COPYRIGHT = 2017
ARCHIVE = textseeker
TEMPLATE = app
VERSION = 1.0.1
CONFIG += c++11 warning widgets gui core
QT += widgets

xcode_export:CONFIG -= app_bundle

unix {
    isEmpty(PREFIX):PREFIX=$$system(echo $$[QT_INSTALL_DATA] | sed s:/[a-z0-9]*/qt5$::)
    !macx:CONFIG += make-install make-desktop
}

macx {
    equals(PREFIX, "/usr/local")|equals(PREFIX, "/opt/local") {
        CONFIG -= app_bundle
        CONFIG += make-install
    }
    else {  # if .pro updated, always rebuild osx app shell...
        QMAKE_MACOSX_DEPLOYMENT_TARGET=10.10
        system(rm -rf $${OUT_PWD}/$${TARGET}.app)
    }
}

win32 {
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /debug
    CONFIG -= debug_and_release debug_and_release_target
    CONFIG += skip_target_version_ext
    DEFINES += WIN32_LEAN_AND_MEAN
    system(lrelease -silent *.pro)
}
else {
    exists(/usr/bin/lrelease-qt5):system(lrelease-qt5 -silent *.pro)
    else:system(lrelease -silent *.pro)
}

CONFIG(release,release|debug) {
    DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG
    macx:CONFIG+=force_debug_info separate_debug_info
}
else:!no-testdata:DEFINES += PROJECT_TESTDATA=\"$${PWD}/testdata\"

DEFINES += PROJECT_VERSION=$${VERSION} PROJECT_COPYRIGHT=$${COPYRIGHT}
TRANSLATIONS += \
    translate/textseeker_es.ts

exists(Archive/Archive.pri):include(Archive/Archive.pri)

include(src/Main.pri)
include(src/Utils.pri)
include(src/Viewer.pri)

make-install {
    QMAKE_EXTRA_TARGETS += target locale
    INSTALLS += target locale

    target.path = $$PREFIX/bin
    target.depends = all

    locale.files = $${PWD}/translate/*.qm
    locale.path = $$PREFIX/share/translations
    locale.depends = target
}

make-desktop {
    QMAKE_EXTRA_TARGETS += desktop pixmaps
    INSTALLS += desktop pixmaps appdata translate

    desktop.files = $${PWD}/xdg/*.desktop
    desktop.path = $$PREFIX/share/applications
    desktop.depends = target

    pixmaps.files = $${PWD}/xdg/*.png
    pixmaps.path = $$PREFIX/share/pixmaps
    pixmaps.depends = target

    appdata.files = $${PWD}/xdg/*.appdata.xml
    appdata.path = $$PREFIX/share/appdata
    appdata.depends = target

    translate.files = $${PWD}/translate/*.qm
    translate.path = $$PREFIX/share/translations
    translate.depends = target
}

RESOURCES += TextSeeker.qrc
OTHER_FILES += \
    scripts/xcode-gen.sh \
    scripts/vs2013-gen.cmd \
    scripts/lrelease-gen.sh \
    scripts/qbuild.sh \
    xdg/textseeker.png \
    xdg/textseeker.desktop \
    xdg/textseeker.appdata.xml \
    translate/textseeker_es.ts \
    TextSeeker.plist \
    TextSeeker.icns \
    TextSeeker.ico \
    textseeker.spec \
    CHANGELOG \
    LICENSE \
    README.md \
    CONTRIBUTING.md \

QMAKE_TARGET_COMPANY = "Tycho Softworks"
QMAKE_TARGET_COPYRIGHT = "$${COPYRIGHT} Tycho Softworks"
QMAKE_TARGET_PRODUCT = "TextSeeker"
QMAKE_TARGET_DESCRIPTION = "Search and view text content"

macx:QMAKE_INFO_PLIST = TextSeeker.plist
macx:ICON = TextSeeker.icns
win32:RC_ICONS += TextSeeker.ico

