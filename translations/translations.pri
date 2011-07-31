
# A line must be added here for each new translation file.
TRANSLATIONS = \
    translations/hi-IN.ts



isEmpty(QMAKE_LRELEASE) {
    QMAKE_LRELEASE = $$replace(QMAKE_QMAKE,qmake,lrelease)
}

LOCALES_BUILD_DIR=_build/.compiled/locales

compile-translations.input = TRANSLATIONS
compile-translations.output = $${LOCALES_BUILD_DIR}/${QMAKE_FILE_BASE}.qm
compile-translations.commands = $${QMAKE_LRELEASE} -silent \
    translations/${QMAKE_FILE_BASE}.ts -qm $${LOCALES_BUILD_DIR}/${QMAKE_FILE_BASE}.qm
compile-translations.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += compile-translations
