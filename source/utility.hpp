
#ifndef UTILITY_HPP
#define UTILITY_HPP

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
}

#endif
