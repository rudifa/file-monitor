
win32 { 
    RC_FILE = deployment/windows/$${PROJECT_NAME}-icon.rc
    isEmpty(PREFIX):PREFIX = "C:/Program Files"
    BINDIR = $${PREFIX}/$${PROJECT_NAME}
    INSTALLS += target \
        translations \
        license \
        readme
    target.path = $${BINDIR}
    translations.path = $${BINDIR}
    translations.files += _build/.compiled/locales
    license.path = $${BINDIR}
    license.files += documentation/license
    readme.path = $${BINDIR}
    readme.files += documentation/readme
}
mac:

# ICON = browser.icns
# QMAKE_INFO_PLIST = Info_mac.plist
unix { 
    isEmpty(PREFIX):PREFIX = /usr
    BINDIR = $${PREFIX}/bin
    DATADIR = $${PREFIX}/share
    PKGDATADIR = $${DATADIR}/$${PROJECT_NAME}
    DEFINES += DATADIR=\\\"$${DATADIR}\\\" \
        PKGDATADIR=\\\"$${PKGDATADIR}\\\"
    !mac { 
        INSTALLS += target \
            translations \
            desktop \
            iconxpm \
            iconsvg \
            icon16 \
            icon32 \
            icon128 \
            copyright \
            changelog \
            changelog-compress \
            man \
            man-compress
        target.path = $${BINDIR}
        translations.path = $${PKGDATADIR}
        translations.files += _build/.compiled/locales
        desktop.path = $${DATADIR}/applications
        desktop.files += deployment/unix/$${PROJECT_NAME}.desktop
        iconxpm.path = $${DATADIR}/pixmaps
        iconxpm.files += icons/$${PROJECT_NAME}.xpm
        iconsvg.path = $${DATADIR}/icons/hicolor/scalable/apps
        iconsvg.files += icons/$${PROJECT_NAME}.svg
        icon16.path = $${DATADIR}/icons/hicolor/16x16/apps
        icon16.files += icons/16x16/$${PROJECT_NAME}.png
        icon32.path = $${DATADIR}/icons/hicolor/32x32/apps
        icon32.files += icons/32x32/$${PROJECT_NAME}.png
        icon128.path = $${DATADIR}/icons/hicolor/128x128/apps
        icon128.files += icons/128x128/$${PROJECT_NAME}.png
        copyright.path = $${DATADIR}/doc/$${PROJECT_NAME}
        copyright.files += deployment/unix/debian/copyright
        
        # BUG - Source installation's "make uninstall" will not remove the changelog file "changelog.gz".
        changelog.path = $${DATADIR}/doc/$${PROJECT_NAME}
        changelog.files += deployment/unix/debian/changelog

        changelog-compress.path = $${DATADIR}/doc/$${PROJECT_NAME}
        changelog-compress.extra = "" \
            "gzip -9 -f \$(INSTALL_ROOT)/$${DATADIR}/doc/$${PROJECT_NAME}/changelog" \
            ""
        changelog-compress.depends = install_changelog
        
        # BUG - Source installation's "make uninstall" will not remove the man file "$${PROJECT_NAME}.1.gz".
        man.path = $${DATADIR}/man/man1
        man.files += documentation/$${PROJECT_NAME}.1

        man-compress.path = $${DATADIR}/man/man1
        man-compress.extra = "" \
            "gzip -9 -f \$(INSTALL_ROOT)/$${DATADIR}/man/man1/$${PROJECT_NAME}.1" \
            ""
        man-compress.depends = install_man
    }
}

