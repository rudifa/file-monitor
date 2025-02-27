
#include "utility.hpp"

#include <QLocale>
#include <QFileInfo>
#include <QDirIterator>
#include <QApplication>
#include <QSettings>
#include <QDomDocument>
#include <QStringList>

#include <cmath>
#include <cassert>

using namespace utility;

namespace
{
    QString getLocalesDirectory(QApplication const &app)
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

        // TODO: Log this failure.
        // Locale directory could not be found.
        return QString();
    }
    inline QString languageLocaleToTranslationUri(QApplication const &app, QString const &language_locale)
    {
        return getLocalesDirectory(app) + "/" + language_locale + ".qm";
    }
    inline QString QLocaleToXmlLangString(QLocale const &locale)
    {
        return locale.name().replace("_", "-");
    }
    inline QString langFromXmlLangString(QString const &xml_lang)
    {
        return xml_lang.section("-", 0, 0);
    }

    // A sigmoid curve that returns values between 0 and 1.
    inline double logisticFunction(double input)
    {
        double const e = 2.71828182845904523536028747135266249;
        return (1.0 / (1.0 + std::pow(e, -input)));
    }

    // Inverse of logistic function.
    inline double logitFunction(double input)
    {
        return log(input / (1.0 - input));
    }
}

QString locale::getSystemTranslationUri(QApplication const &app)
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
    while (it.hasNext())
    {
        it.next();
        QString file_base_name = it.fileInfo().baseName();
        if (file_base_name.startsWith(language))
            return ::languageLocaleToTranslationUri(app, file_base_name);
    }

    return QString();
}

void settings::saveSettingsToFile(QString const &file_uri)
{
    QSettings global_settings;
    QSettings file_settings(file_uri, QSettings::IniFormat);

    auto keys = global_settings.allKeys();
    for (auto it = keys.begin(); it != keys.end(); ++it)
        file_settings.setValue(*it, global_settings.value(*it));
}

bool settings::loadSettingsFromFile(QString const &file_uri)
{
    QSettings file_settings(file_uri, QSettings::IniFormat);
    auto temp_keys = file_settings.allKeys();
    if (temp_keys.isEmpty())
        return false;

    QSettings global_settings;
    global_settings.clear();

    for (auto it = temp_keys.begin(); it != temp_keys.end(); ++it)
        global_settings.setValue(*it, file_settings.value(*it));

    return true;
}

double math::scaleValueBetweenRanges(double input, double input_min,
                                     double input_max, double output_min, double output_max)
{
    double input_range = (input_max - input_min);
    double output_range = (output_max - output_min);
    double input_scale = (output_range / input_range);

    // 1. Shift input_min to zero (for scaling).
    // 2. Scale the input.
    // 3. Shift the resulting value to the ouptut minimum.
    return (((input - input_min) * input_scale) + output_min);
}

double math::rampUp(double input, double input_min, double input_max, double output_min, double output_max)
{
    // We start the min a little low to try to get us very close to the output_min.
    double const logistic_function_input_min = -10;
    // Stop half way through the logistic curve so that we only include the slow start and steady ramp up.
    double const logistic_function_input_max = 0;

    double scaled_input = scaleValueBetweenRanges(input, input_min, input_max,
                                                  logistic_function_input_min, logistic_function_input_max);

    double logistic_function_output = logisticFunction(scaled_input);

    double const logistic_function_output_min = 0;
    // We are only moving half-way through the logistic function, so the output is .5 rather than 1.0.
    double const logistic_function_output_max = .5;

    double scaled_logistic_function_output = scaleValueBetweenRanges(logistic_function_output,
                                                                     logistic_function_output_min, logistic_function_output_max, output_min, output_max);
    return scaled_logistic_function_output;
}

double math::inverseRampUp(double input, double input_min, double input_max, double output_min, double output_max)
{
    // We start the min a little low to try to get us very close to the output_min.
    double const logit_function_input_min = 0;
    // Stop half way through the logistic curve so that we only include the slow start and steady ramp up.
    double const logit_function_input_max = .5;

    double scaled_input = scaleValueBetweenRanges(input, input_min, input_max,
                                                  logit_function_input_min, logit_function_input_max);

    double logit_function_output = logitFunction(scaled_input);

    double const logit_function_output_min = -10;
    // We are only moving half-way through the logistic function, so the output is .5 rather than 1.0.
    double const logit_function_output_max = 0;

    double scaled_logit_function_output = scaleValueBetweenRanges(logit_function_output,
                                                                  logit_function_output_min, logit_function_output_max, output_min, output_max);

    return scaled_logit_function_output;
}

bool xml::isXML(QString const &content)
{
    QDomDocument document;
    QDomDocument::ParseResult result = document.setContent(content);
    return static_cast<bool>(result);
}

QStringList qt_extensions::operator-(QStringList lhs, QStringList const &rhs)
{
    for (int i = 0; i < rhs.count(); ++i)
        lhs.removeAll(rhs[i]);

    return lhs;
}
