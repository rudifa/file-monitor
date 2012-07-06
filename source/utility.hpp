
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QPointF>

#include <cmath>

class QApplication;
class QString;

namespace utility
{
    namespace locale
    {
        QString getSystemTranslationUri(QApplication const & app);
    }
    namespace settings
    {
        void saveSettingsToFile(QString const & file_uri);
        bool loadSettingsFromFile(QString const & file_uri);
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
        bool isXML(QString const & content);
    }
}

#endif
