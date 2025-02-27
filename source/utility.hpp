
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QtCore>
#include <QtGui>

#include <cmath>
#include <functional>
#include <algorithm>

namespace utility
{
    namespace locale
    {
        QString getSystemTranslationUri(QApplication const &app);
    }
    namespace settings
    {
        void saveSettingsToFile(QString const &file_uri);
        bool loadSettingsFromFile(QString const &file_uri);
    }
    namespace math
    {
        inline double round(double input) { return input < 0.0 ? std::ceil(input - 0.5) : std::floor(input + 0.5); }

        double scaleValueBetweenRanges(double input, double input_min, double input_max, double output_min, double output_max);

        // First half of the logistic function that starts slowly and steadily ramps upward.
        double rampUp(double input, double input_min, double input_max, double output_min, double output_max);
        double inverseRampUp(double input, double input_min, double input_max, double output_min, double output_max);
    }
    namespace xml
    {
        bool isXML(QString const &content);
    }
    namespace qt_extensions
    {
        QStringList operator-(QStringList lhs, QStringList const &rhs);
    }
    namespace file
    {
        struct OnDisk
        {
            // using argument_type = QString;
            // using result_type = bool;

            bool operator()(const QString &path) const
            {
                return QFileInfo(path).exists();
            }
        };

        // Return a collection of only the uris that have a corresponding file on disk.
        template <typename T>
        inline T clearUrisWithNoFileOnDisk(T file_uris)
        {
            static QFileInfo file_info;
            file_uris.erase(
                std::remove_if(
                    file_uris.begin(),
                    file_uris.end(),
                    [](const QString &path)
                    { return !OnDisk()(path); }),
                file_uris.end());
            return file_uris;
        }

        // Return a collection of only the uris that do not have a corresponding file on disk.
        template <typename T>
        inline T clearUrisWithFileOnDisk(T file_uris)
        {
            static QFileInfo file_info;
            file_uris.erase(
                std::remove_if(file_uris.begin(), file_uris.end(), OnDisk()),
                file_uris.end());
            return file_uris;
        }
    }
}

#endif
