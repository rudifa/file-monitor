
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

namespace
{
   TabPage::FileType determineFileType(QString const & file_uri)
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

TabPage::TabPage(Ui::MainWindow const & ui, QWidget * parent)
    : QWidget(parent), ui(ui), displayed_to_user(false)
{
}

bool TabPage::load(QString const & uri)
{
    assert(!uri.isEmpty());

    file_uri = uri;
    file_type = ::determineFileType(uri);
    view = createView(file_uri);
    setStatusTip(file_uri);

    QGridLayout * layout = new QGridLayout(this);
    layout->setMargin(4);
    layout->addWidget(view->getWidget());

    connect(view, SIGNAL(signalScaleChanged()), SIGNAL(signalScaleChanged()));
    connect(this, SIGNAL(signalFindNext(QString const &, bool)), view, SLOT(slotFindNext(QString const &, bool)));
    connect(this, SIGNAL(signalFindPrevious(QString const &, bool)), view, SLOT(slotFindPrevious(QString const &, bool)));

    QString file_path_delimiter("*");
    settings.beginGroup("files/" + file_path_delimiter + getUri());
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

void TabPage::scrollToBottomOnChange(bool scroll_to_bottom)
{
    view->scrollToBottomOnChange(scroll_to_bottom);
}

QString TabPage::getUri() const
{
    return file_uri;
}

bool TabPage::isImage() const
{
    return (file_type == TabPage::SVG || file_type == TabPage::IMAGE);
}

int TabPage::getPercentageZoom() const
{
    return view->getZoom();
}

void TabPage::slotZoomIn()
{
    double zoom = view->getZoom() + zoom::step;
    slotSetZoom(zoom);
}

void TabPage::slotZoomOut()
{
    double zoom = view->getZoom() - zoom::step;
    slotSetZoom(zoom);
}

void TabPage::slotResetZoom()
{
    view->resetZoom();
}

void TabPage::slotSelectAll()
{
    view->selectAll();
}

void TabPage::slotCopy()
{
    view->copy();
}

void TabPage::slotSetZoom(int zoom)
{
    if (!zoomIsValid(zoom))
        return;

    view->setZoom(zoom);
}

void TabPage::slotLoadSettings()
{
    if (!wasCurrentTab())
        return;

    bool block_signals = blockSignals(true);

    view->setZoom(settings.value("zoom", 0).toInt());
    view->setScrollDimensions(settings.value("scroll_dimensions", QPoint(0, 0)).toPoint());

    blockSignals(block_signals);
}

void TabPage::slotReload()
{
    slotSaveSettings();
    view->load(file_uri, true);
    slotLoadSettings();
}

void TabPage::slotSaveSettings()
{
    if (!wasCurrentTab())
        return;

    settings.setValue("zoom", view->getZoom());
    settings.setValue("scroll_dimensions", view->getScrollDimensions());
}

View * TabPage::createView(QString const & file_uri)
{
    switch (::determineFileType(file_uri))
    {
        case TabPage::HTML:
            return new HtmlView(ui, this);
        case TabPage::SVG:
            return new SvgView(ui, this);
        case TabPage::IMAGE:
            return new ImageView(ui, this);
        
        case TabPage::TEXT:
        default:
            return new TextView(ui, this);
    }
}

bool TabPage::zoomIsValid(double zoom) const
{
    return (zoom >= zoom::min && zoom <= zoom::max);
}
