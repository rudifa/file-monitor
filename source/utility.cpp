
#include "utility.hpp"

#include <QLocale>
#include <QFileInfo>
#include <QDirIterator>
#include <QApplication>

#include <cassert>

namespace
{
    QString getLocalesDirectory(QApplication const & app)
    {
	// On windows and during builds on unix, the locale folder is next to the app.
	QString locales_dir("/locales");
        QString path(app.applicationDirPath() + locales_dir);
        QFileInfo file_info(path);
        if (file_info.exists())
            return path;

        // If installed on unix, the locale files are in the PKGDATADIR as set by deployment.pri.
        #ifdef PKGDATADIR
            return QString(PKGDATADIR) + locales_dir;
        #endif

//TODO: Log this failure.
	// Locale directory could not be found.
	return QString();
    }
    QString languageLocaleToTranslationUri(QApplication const & app,
        QString const & language_locale)
    {
        return getLocalesDirectory(app) + "/" + language_locale + ".qm";
    }
    QString QLocaleToXmlLangString(QLocale const & locale)
    {
        return locale.name().replace("_", "-");
    }
    QString langFromXmlLangString(QString const & xml_lang)
    {
        return xml_lang.section("-", 0, 0);
    }
}

QString utility::locale::getSystemTranslationUri(QApplication const & app)
{
    // Convert from Qt to xml:lang language-locale convention.
    QString xml_lang = QLocaleToXmlLangString(QLocale::system());
    QString tranlation_uri = ::languageLocaleToTranslationUri(app, xml_lang);
    
    // Look for a matching language-locale.
    QFileInfo file_info(tranlation_uri);
    if (file_info.exists())
        return tranlation_uri;
    
    // Look for a matching language.
    QString language = langFromXmlLangString(xml_lang);
    QString translations_directory = getLocalesDirectory(app);
    QDirIterator it(translations_directory);
    while (it.hasNext()) {
        it.next();
        QString file_base_name = it.fileInfo().baseName();
        if (file_base_name.startsWith(language))
            return ::languageLocaleToTranslationUri(app, file_base_name);
    }

    return QString();
}
