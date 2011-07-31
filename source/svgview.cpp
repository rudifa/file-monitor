
#include "svgview.hpp"

#include "tabpage.hpp"

#include <QGraphicsSvgItem>
#include <QWheelEvent>
#include <QScrollBar>

#include <cmath>
#include <cassert>

SvgView::SvgView(QWidget * parent)
    : QGraphicsView(parent), View(ZoomParameters(.3, 1.5, 3))
{
    setScene(new QGraphicsScene(this));
    setTransformationAnchor(AnchorUnderMouse);
    setDragMode(ScrollHandDrag);
    setViewportUpdateMode(FullViewportUpdate);
}

SvgView::~SvgView()
{
}

QWidget * SvgView::getWidget()
{
    QWidget * widget = dynamic_cast<QWidget *>(this);
    assert(widget);

    return widget;
}

bool SvgView::load(QString const & file_uri)
{
    QGraphicsScene * graphics_scene = scene();

    graphics_scene->clear();
    resetTransform();

    graphics_item = new QGraphicsSvgItem(file_uri);
    graphics_item->setFlags(QGraphicsItem::ItemClipsToShape);
    graphics_item->setCacheMode(QGraphicsItem::NoCache);
    graphics_item->setZValue(0);

    graphics_scene->addItem(graphics_item);
    graphics_scene->setSceneRect(graphics_item->boundingRect());
    return true;
}

void SvgView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

void SvgView::setVerticalScroll(int scroll)
{
    verticalScrollBar()->setSliderPosition(scroll);
}

int SvgView::getVerticalScroll() const
{
    return verticalScrollBar()->value();
}

void SvgView::setHorizontalScroll(int scroll)
{
    horizontalScrollBar()->setValue(scroll);
}

int SvgView::getHorizontalScroll() const
{
    return horizontalScrollBar()->value();
}

void SvgView::setZoom(double zoom)
{
    if (zoom == 0.0)
        zoom = zoom_parameters.initial;

    double relative_zoom = zoom * zoom_parameters.scale + zoom_parameters.offset;
    resetTransform();
    scale(relative_zoom, relative_zoom);

    absolute_zoom = zoom;
}
