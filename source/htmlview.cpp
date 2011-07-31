
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
{
    connect(web_view->page()->currentFrame(),
        SIGNAL(initialLayoutCompleted()), SLOT(slotSetScroll()));
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

#include <QDebug>

void HtmlView::setVerticalScroll(int scroll)
{


    qDebug() << web_view->url() << " - setVerticalScroll: " << scroll;
    deferred_vertical_scroll = scroll;
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

void HtmlView::slotSetScroll()
{

    qDebug() << web_view->url() << " - slotSetScroll: " << deferred_vertical_scroll;

    QTimer::singleShot(100, this, SLOT(slotLoadDeferredScroll()));
}

void HtmlView::slotLoadDeferredScroll()
{
//    if (!web_view->cu)
//    {
        QPoint scroll = QPoint(deferred_horizontal_scroll, deferred_vertical_scroll);
        web_view->page()->currentFrame()->setScrollPosition(scroll);
//    } else {
//        QTimer::singleShot(10, this, SLOT(slotLoadDeferredScroll()));
//    }



}

void HtmlView::setZoom(double zoom)
{
    if (zoom == 0.0)
        zoom = zoom_parameters.initial;

    double relative_zoom = zoom * zoom_parameters.scale + zoom_parameters.offset;
    web_view->page()->currentFrame()->setZoomFactor(relative_zoom);

    absolute_zoom = zoom;
}
