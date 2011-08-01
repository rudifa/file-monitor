
#include "svgview.hpp"

#include "tabpage.hpp"

#include <QGraphicsSvgItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QTimer>

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

void SvgView::setZoom(double zoom)
{
    if (zoom == 0.0)
        zoom = zoom_parameters.initial;

    double relative_zoom = zoom * zoom_parameters.scale + zoom_parameters.offset;
    resetTransform();
    scale(relative_zoom, relative_zoom);

    absolute_zoom = zoom;
}

void SvgView::setScrollDimensions(QPoint dimensions)
{
    scroll_dimensions = dimensions;
    slotSetScroll();
}

QPoint SvgView::getScrollDimensions() const
{
    int horizontal_scroll = horizontalScrollBar()->value();
    int vertical_scroll = verticalScrollBar()->value();
    return QPoint(horizontal_scroll, vertical_scroll);
}

//  Periodically check to see if the control is ready for the scroll to be set.
void SvgView::slotSetScroll()
{
    int max_scroll_vertical = verticalScrollBar()->maximum();
    int max_scroll_horizontal = horizontalScrollBar()->maximum();
    QPoint scroll_max(max_scroll_horizontal, max_scroll_vertical);

    if (scroll_max.x() >= scroll_dimensions.x() && scroll_max.y() >= scroll_dimensions.y())
    {
        horizontalScrollBar()->setSliderPosition(scroll_dimensions.x());
        verticalScrollBar()->setSliderPosition(scroll_dimensions.y());
    } else {
        QTimer::singleShot(10, this, SLOT(slotSetScroll()));
    }
}
