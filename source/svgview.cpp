
#include "svgview.hpp"

#include "tabpage.hpp"
#include "customgraphicsview.hpp"
#include "utility.hpp"

#include <QGraphicsSvgItem>
#include <QScrollBar>
#include <QWheelEvent>

#include <cmath>
#include <cassert>

using namespace utility;

SvgView::SvgView(QWidget * parent)
    : View(parent, ZoomConfiguration(ZoomConfiguration::RampUp, .1, 500, 1)), graphics_view(new CustomGraphicsView(parent))
{
    graphics_view->setAcceptDrops(false);
    graphics_view->setScene(new QGraphicsScene(this));
    graphics_view->setTransformationAnchor(graphics_view->AnchorUnderMouse);
    graphics_view->setDragMode(graphics_view->ScrollHandDrag);
    graphics_view->setViewportUpdateMode(graphics_view->SmartViewportUpdate);

    connect(graphics_view->verticalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
    connect(graphics_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
    connect(graphics_view, SIGNAL(signalZoomIn()), SIGNAL(signalZoomIn()));
    connect(graphics_view, SIGNAL(signalZoomOut()), SIGNAL(signalZoomOut()));
}

QWidget * SvgView::getWidget()
{
    QWidget * widget = dynamic_cast<QWidget *>(graphics_view);
    assert(widget);

    return widget;
}

bool SvgView::load(QString const & file_uri)
{
    QGraphicsScene * graphics_scene = graphics_view->scene();

    graphics_scene->clear();
    graphics_view->resetTransform();

    graphics_item = new QGraphicsSvgItem(file_uri);
    graphics_item->setFlags(QGraphicsItem::ItemClipsToShape);
    graphics_item->setCacheMode(QGraphicsItem::NoCache);
    graphics_item->setZValue(0);

    graphics_scene->addItem(graphics_item);
    graphics_scene->setSceneRect(graphics_item->boundingRect());
    return true;
}

void SvgView::setZoom(double zoom)
{
    graphics_view->resetTransform();
    graphics_view->scale(zoom, zoom);
}

void SvgView::setScrollDimensions(QPoint dimensions)
{
    graphics_view->horizontalScrollBar()->setSliderPosition(dimensions.x());
    graphics_view->verticalScrollBar()->setSliderPosition(dimensions.y());
}

QPoint SvgView::getScrollDimensions() const
{
    int horizontal_scroll = graphics_view->horizontalScrollBar()->value();
    int vertical_scroll = graphics_view->verticalScrollBar()->value();
    return QPoint(horizontal_scroll, vertical_scroll);
}

void SvgView::enableTransparentBackground(bool enable)
{
    graphics_view->setBackgroundBrush(enable ? QBrush(transparent_tile_pixmap) : Qt::NoBrush);
}
