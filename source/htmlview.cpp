
#include "htmlview.hpp"

#include <QWebView>
#include <QWebFrame>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QTimer>

#include <cmath>
#include <cassert>

HtmlView::HtmlView(QWidget * parent)
    : QObject(parent), View(ZoomParameters(.03, .5, 17)),
    web_view(new QWebView(parent))
{ }

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

void HtmlView::setVerticalScroll(int scroll)
{
    deferred_vertical_scroll = scroll;
    slotLoadDeferredScroll();
}

int HtmlView::getVerticalScroll() const
{
    return web_view->page()->currentFrame()->scrollPosition().y();
}

void HtmlView::setHorizontalScroll(int scroll)
{
    deferred_horizontal_scroll = scroll;
}

int HtmlView::getHorizontalScroll() const
{
    return web_view->page()->currentFrame()->scrollPosition().x();
}

// QWebView has no signal to tell us that the page has fully loaded.
//  Instead, we periodically check to see if it is ready for the scroll to be set.
void HtmlView::slotLoadDeferredScroll()
{
    if (web_view->page()->currentFrame()->scrollBarMaximum(Qt::Vertical) >= deferred_vertical_scroll)
    {
        QPoint scroll = QPoint(deferred_horizontal_scroll, deferred_vertical_scroll);
        web_view->page()->currentFrame()->setScrollPosition(scroll);
    } else {
        QTimer::singleShot(10, this, SLOT(slotLoadDeferredScroll()));
    }
}

void HtmlView::setZoom(double zoom)
{
    if (zoom == 0.0)
        zoom = zoom_parameters.initial;

    double relative_zoom = zoom * zoom_parameters.scale + zoom_parameters.offset;
    web_view->page()->currentFrame()->setZoomFactor(relative_zoom);

    absolute_zoom = zoom;
}
