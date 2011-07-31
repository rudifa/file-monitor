
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QString>

class QApplication;

namespace utility
{
namespace locale
{
    QString getSystemTranslationUri(QApplication const & app);
}
}

#endif
