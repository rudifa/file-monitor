
#include "tabpage.hpp"

#include "view.hpp"
#include "textview.hpp"
#include "svgview.hpp"
#include "htmlview.hpp"
#include "imageview.hpp"

#include <QGridLayout>
#include <QWheelEvent>
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
      if (extension == "bmp" || extension == "gif" || extension == "jpg" ||
          extension == "jpeg" || extension == "png" || extension == "pbm" ||
          extension == "pgm" || extension == "ppm" || extension == "xbm" ||
          extension == "xpm" || extension =="ico")
      {
          return TabPage::IMAGE;
      }

      return TabPage::TEXT;
   }
}

TabPage::TabPage(QWidget * parent)
    : QWidget(parent), displayed_to_user(false)
{
}

bool TabPage::load(QString const & uri)
{
    assert(!uri.isEmpty());

    file_uri = uri;
    view = createView(file_uri);
    setStatusTip(file_uri);

    // TODO: remove this section next - add QScrollArea.
    QGridLayout * layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(view->getWidget());

    connect(view, SIGNAL(signalZoomIn()), SLOT(slotZoomIn()));
    connect(view, SIGNAL(signalZoomOut()), SLOT(slotZoomOut()));

    settings.beginGroup("files/" + getUri());
    return view->load(file_uri);
}

void TabPage::enableTransparentBackground(bool enable)
{
    view->enableTransparentBackground(enable);
}

void TabPage::wordWrap(bool word_wrap)
{
    view->wordWrap(word_wrap);
}

void TabPage::indentXML(bool indent_xml)
{
    view->indentXML(indent_xml);
}

QString TabPage::getUri() const
{
    return file_uri;
}

int TabPage::getPercentageZoom() const
{
    return view->getPercentageZoom();
}

void TabPage::slotZoomIn()
{
    double zoom = view->getPercentageZoom() + TabPage::zoom_step;
    slotSetZoom(zoom);
}

void TabPage::slotZoomOut()
{
    double zoom = view->getPercentageZoom() - TabPage::zoom_step;
    slotSetZoom(zoom);
}

void TabPage::slotResetZoom()
{
    view->resetPercentageZoom();
    signalUserChangedZoom(view->getPercentageZoom());
}

void TabPage::slotSetZoom(int zoom)
{
    if (!zoomIsValid(zoom))
        return;

    view->setPercentageZoom(zoom);
    signalUserChangedZoom(zoom);
}

void TabPage::slotLoadSettings()
{
    if (!wasCurrentTab())
        return;

    view->setPercentageZoom(settings.value("zoom", 0).toInt());
    view->setScrollDimensions(settings.value("scroll_dimensions", QPoint(0, 0)).toPoint());
}

void TabPage::slotReload()
{
    slotSaveSettings();
    view->load(file_uri);
    slotLoadSettings();
}

void TabPage::slotSaveSettings()
{
    if (!wasCurrentTab())
        return;

    settings.setValue("zoom", view->getPercentageZoom());
    settings.setValue("scroll_dimensions", view->getScrollDimensions());
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
