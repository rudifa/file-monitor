
#include "tabpage.hpp"

#include "textview.hpp"
#include "svgview.hpp"
#include "htmlview.hpp"
#include "imageview.hpp"

#include <QFileSystemWatcher>
#include <QGridLayout>
#include <qmath.h>

#include <cassert>

double const TabPage::zoom_min = 1;
double const TabPage::zoom_max = 100;
double const TabPage::zoom_step = 2;

namespace
{
   TabPage::FileType getFileType(QString const & file_uri)
   {
      int last_dot_index = file_uri.lastIndexOf('.');
      int last_slash_index = file_uri.lastIndexOf('/');
      if (last_slash_index > last_dot_index || last_dot_index == -1)
         return TabPage::TEXT;

      QString extension = file_uri.mid(last_dot_index + 1,
         file_uri.size() - last_dot_index).toLower();

      if (extension == "html" || extension == "htm")
         return TabPage::HTML;
      if (extension == "svg")
         return TabPage::SVG;
      if (extension == "jpg" || extension == "jpeg" || extension == "gif"
          || extension == "png" || extension == "bmp" || extension =="ico") {
          return TabPage::IMAGE;
      }

      return TabPage::TEXT;
   }
}

TabPage::TabPage(QWidget * parent)
    : QWidget(parent), file_watcher(0)
{
}

TabPage::~TabPage()
{
    saveSettings();
}

bool TabPage::load(QString const & uri)
{
    assert(!uri.isEmpty());

    file_uri = uri;
    view = createView(file_uri);
    setStatusTip(file_uri);

    file_watcher = new QFileSystemWatcher(this);
    file_watcher->addPath(file_uri);

    QGridLayout * layout = new QGridLayout(this);
    layout->setContentsMargins(0, 4, 0, 4);
    layout->addWidget(view->getWidget());

    return view->load(file_uri);
}

QString TabPage::getUri() const
{
    return file_uri;
}

QFileSystemWatcher * TabPage::getFileWatcher()
{
    return file_watcher;
}

int TabPage::getZoom() const
{
    return round(view->getZoom());
}

void TabPage::slotZoomIn()
{
    double zoom = view->getZoom() + TabPage::zoom_step;
    if (!zoomIsValid(zoom))
        return;

    view->setZoom(zoom);
    signalZoomChanged(zoom);
}

void TabPage::slotZoomOut()
{
    double zoom = view->getZoom() - TabPage::zoom_step;
    if (!zoomIsValid(zoom))
        return;

    view->setZoom(zoom);
    signalZoomChanged(zoom);
}

void TabPage::slotSetZoom(int zoom)
{
    if (!zoomIsValid(zoom))
        return;

    view->setZoom(zoom);
}

void TabPage::loadSettings()
{
    settings.beginGroup(getUri());
    view->setZoom(settings.value("zoom", 0).toDouble());
    view->setScrollDimensions(settings.value("scroll_dimensions", QPoint(0, 0)).toPoint());
    settings.endGroup();

    signalZoomChanged(view->getZoom());
}

void TabPage::slotReload()
{
    saveSettings();
    view->load(file_uri);
    loadSettings();
}

void TabPage::saveSettings()
{
    settings.beginGroup(getUri());
    settings.setValue("zoom", view->getZoom());
    settings.setValue("scroll_dimensions", view->getScrollDimensions());
    settings.endGroup();
}

View * TabPage::createView(QString const & file_uri)
{
    switch (getFileType(file_uri))
    {
        case TabPage::HTML:
            return new HtmlView(this);
        case TabPage::SVG:
            return new SvgView(this);
        case TabPage::IMAGE:
            return new ImageView(this);
        
        case TabPage::TEXT:
        default:
            return new TextView(this);
    }
}

bool TabPage::zoomIsValid(double zoom) const
{
    return (zoom >= TabPage::zoom_min && zoom <= TabPage::zoom_max);
}
