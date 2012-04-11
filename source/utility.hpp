
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
}

#endif
