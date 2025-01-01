
#include "utility.hpp"
#include "mainwindow.hpp"

#include <QApplication>
#include <QTranslator>

using namespace utility;

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("XmlSoft");
    QCoreApplication::setOrganizationDomain("xmlsoft.com");
    QCoreApplication::setApplicationName("File Monitor");

    QApplication app(argc, argv);
    QTranslator translator;

    QString translation_uri = locale::getSystemTranslationUri(app);
    if (!translation_uri.isEmpty())
    {
        translator.load(translation_uri);
        app.installTranslator(&translator);
    }

    MainWindow window;
    window.show();

    return app.exec();
}
