
#include "htmlview.hpp"

#include <QWebView>
#include <QWebFrame>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QTimer>

#include <cmath>
#include <cassert>

#include <utility>

HtmlView::HtmlView(QWidget * parent)
    : View(parent, ZoomParameters(.03, .5, 17)), web_view(new QWebView(parent))
{
    connect(web_view->page(), SIGNAL(scrollRequested(int,int,QRect)), SIGNAL(signalUserChangedDisplay()));
}

HtmlView::~HtmlView()
{
}

QWidget * HtmlView::getWidget()
{
    return web_view;
}

bool HtmlView::load(QString const & file_uri)
{
    web_view->setUrl(file_uri);

    return true;
}

void HtmlView::setZoom(double zoom)
{
    if (zoom == 0.0)
        zoom = zoom_parameters.initial;

    double relative_zoom = zoom * zoom_parameters.scale + zoom_parameters.offset;
    web_view->page()->currentFrame()->setZoomFactor(relative_zoom);

    absolute_zoom = zoom;
}

namespace
{
    // Holds the desired scroll dimensions while the HTML control renders.
    QPoint scroll_dimensions;
}

void HtmlView::setScrollDimensions(QPoint dimensions)
{
    scroll_dimensions = dimensions;
    slotSetScroll();
}

// Periodically check to see if the control is ready for the scroll to be set.
void HtmlView::slotSetScroll()
{
    int max_scroll_vertical = web_view->page()->currentFrame()->scrollBarMaximum(Qt::Vertical);
    int max_scroll_horizontal = web_view->page()->currentFrame()->scrollBarMaximum(Qt::Horizontal);
    QPoint scroll_max(max_scroll_horizontal, max_scroll_vertical);

    if (scroll_max.x() >= scroll_dimensions.x() && scroll_max.y() >= scroll_dimensions.y())
        web_view->page()->currentFrame()->setScrollPosition(scroll_dimensions);
    else
        QTimer::singleShot(10, this, SLOT(slotSetScroll()));
}

QPoint HtmlView::getScrollDimensions() const
{
    return web_view->page()->currentFrame()->scrollPosition();
}
