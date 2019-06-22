TEMPLATE = app
VERSION = 1.3.0
COPYRIGHT = 2017-2019
ARCHIVE = textseeker

# global defines
DEFINES += \
    PROJECT_VERSION=\\\"$${VERSION}\\\" \
    PROJECT_COPYRIGHT=\\\"$${COPYRIGHT}\\\" \
    PROJECT_TARGET=\\\"$${TARGET}\\\" \
    PROJECT_ARCHIVE=\\\"$${ARCHIVE}\\\" \

# basic compile and link config
CONFIG += c++11 warning widgets gui core embed_translations lrelease
CONFIG -= debug_and_release
QT += widgets

# build type specific options
CONFIG(release,release|debug):DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG
else:DEFINES += PROJECT_TESTDATA=\\\"$${PWD}/testdata\\\"
exists(.custom/Custom.pri):include(.custom/Custom.pri)

# platform specific options
unix {
    CONFIG += DesktopClient
    !macx:TARGET=textseeker
    isEmpty(PREFIX):PREFIX=$$system(echo $$[QT_INSTALL_DATA] | sed s:/[a-z0-9]*/qt5$::)
    system(rm -f "$${OUT_PWD}/$${TARGET}")
}

macx {
    equals(PREFIX, "/usr/local"):CONFIG -= app_bundle
    equals(PREFIX, "/opt/local"):CONFIG -= app_bundle

    CONFIG(app_bundle) {
        QMAKE_INFO_PLIST = "$${TARGET}.plist"
        ICON = "$${TARGET}.icns"
        system(rm -rf "$${OUT_PWD}/$${TARGET}.app")
    }
    else:TARGET=textseeker
}

win32 {
    CONFIG += DesktopClient skip_target_version_ext app_bundle
    DEFINES += WIN32_LEAN_AND_MEAN
    RC_ICONS += "$${TARGET}.ico"
}

!CONFIG(DesktopClient):error(unsupported platform)

# project layout
OBJECTS_DIR = objects
RCC_DIR = generated
MOC_DIR = generated
UI_DIR = generated

include(src/Common.pri)
include(src/Controls.pri)
include(src/Localize.pri)
include(src/Main.pri)

RESOURCES += qrc/desktop.qrc
OTHER_FILES += \
    debian/changelog \
    debian/control \
    debian/copyright \
    debian/rules \
    debian/textseeker.install \
    debian/watch \
    xdg/$${ARCHIVE}.1 \
    xdg/$${ARCHIVE}.desktop \
    xdg/$${ARCHIVE}.appdata.xml \
    xdg/$${ARCHIVE}.spec.in \
    Doxyfile \
    CHANGELOG \
    LICENSE \
    README.md \
    CONTRIBUTING.md \
    
# source publish
unix {
    QMAKE_EXTRA_TARGETS += source
    source.commands += $$QMAKE_DEL_FILE *.tar.gz &&
    source.commands += cd $${PWD} &&
    source.commands += git archive --output="$${OUT_PWD}/$${ARCHIVE}-$${VERSION}.tar.gz" --format tar.gz  --prefix=$${ARCHIVE}-$${VERSION}/ v$${VERSION} 2>/dev/null || git archive --output="$${OUT_PWD}/$${ARCHIVE}-$${VERSION}.tar.gz" --format tar.gz  --prefix=$${ARCHIVE}-$${VERSION}/ HEAD 
}

# standard build targets...
!CONFIG(app_bundle) {
    QMAKE_EXTRA_TARGETS += target man1 desktop pixmaps appdata
    INSTALLS += target man1 desktop pixmaps appdata

    target.path = "$$PREFIX/bin"
    target.depends = all

    man1.files = xdg/$${ARCHIVE}.1
    man1.path = "$$PREFIX/share/man/man1"
    man1.depends = target

    desktop.files = xdg/$${ARCHIVE}.desktop
    desktop.path = "$$PREFIX/share/applications"
    desktop.depends = target

    pixmaps.files = qrc/$${ARCHIVE}.png
    pixmaps.path = "$$PREFIX/share/pixmaps"
    pixmaps.depends = target

    appdata.files = xdg/$${ARCHIVE}.appdata.xml
    appdata.path = "$$PREFIX/share/metainfo"
    appdata.depends = target        
}

# clean
QMAKE_EXTRA_TARGETS += clean extra_clean
clean.depends += extra_clean
unix:!CONFIG(app_bundle):extra_clean.commands += rm -f ${TARGET}
macx:CONFIG(app_bundle):extra_clean.commands += rm -rf $${TARGET}.app $${TARGET}.app.dSYM
win32:extra_clean.commands += $$QMAKE_DEL_FILE $${TARGET}.exe

# deployment 
QMAKE_TARGET_COMPANY = "Tycho Softworks"
QMAKE_TARGET_COPYRIGHT = "$${COPYRIGHT} Tycho Softworks"
QMAKE_TARGET_PRODUCT = "$${TARGET}"
QMAKE_TARGET_DESCRIPTION = "Search and view text content"

QMAKE_SUBSTITUTES += specs
specs.input = xdg/$${ARCHIVE}.spec.in
specs.output = $$PWD/$${ARCHIVE}.spec

exists(.deploy/Deploy.pri):include(.deploy/Deploy.pri)
else:CONFIG(release,release|debug) {  # public deployment code would be here...
    win32:QMAKE_POST_LINK += windeployqt "$${TARGET}.exe" -verbose=0
    macx:QMAKE_POST_LINK += macdeployqt "$${TARGET}.app" -verbose=0
}
