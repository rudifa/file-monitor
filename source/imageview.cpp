
#include "imageview.hpp"

#include "tabpage.hpp"

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QScrollBar>

#include <cmath>
#include <cassert>

ImageView::ImageView(QWidget * parent)
    : View(parent, ZoomParameters(.05, .3, 14)), graphics_view(new QGraphicsView(parent))
{
    graphics_view->setAcceptDrops(false);
    graphics_view->setScene(new QGraphicsScene(this));
    graphics_view->setTransformationAnchor(graphics_view->AnchorUnderMouse);
    graphics_view->setDragMode(graphics_view->ScrollHandDrag);
    graphics_view->setViewportUpdateMode(graphics_view->FullViewportUpdate);

    connect(graphics_view->verticalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
    connect(graphics_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
}

QWidget * ImageView::getWidget()
{
    QWidget * widget = dynamic_cast<QWidget *>(graphics_view);
    assert(widget);

    return widget;
}

bool ImageView::load(QString const & file_uri)
{
    QGraphicsScene * graphics_scene = graphics_view->scene();

    graphics_scene->clear();
    graphics_view->resetTransform();

    graphics_item = new QGraphicsPixmapItem(file_uri);
    graphics_item->setFlags(QGraphicsItem::ItemClipsToShape);
    graphics_item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    graphics_item->setTransformationMode(Qt::SmoothTransformation);
    graphics_item->setZValue(0);

    graphics_scene->addItem(graphics_item);
    graphics_scene->setSceneRect(graphics_item->boundingRect());
    return true;
}

void ImageView::setZoom(double zoom)
{
    if (zoom == 0.0)
        zoom = zoom_parameters.initial;

    double relative_zoom = zoom * zoom_parameters.scale + zoom_parameters.offset;
    graphics_view->resetTransform();
    graphics_view->scale(relative_zoom, relative_zoom);

    absolute_zoom = zoom;
}

void ImageView::setScrollDimensions(QPoint dimensions)
{
    graphics_view->horizontalScrollBar()->setSliderPosition(dimensions.x());
    graphics_view->verticalScrollBar()->setSliderPosition(dimensions.y());
}

QPoint ImageView::getScrollDimensions() const
{
    int horizontal_scroll = graphics_view->horizontalScrollBar()->value();
    int vertical_scroll = graphics_view->verticalScrollBar()->value();
    return QPoint(horizontal_scroll, vertical_scroll);
}

void ImageView::enableTransparentBackground(bool enable)
{
    graphics_view->setBackgroundBrush(enable ? QBrush(transparent_tile_pixmap) : Qt::NoBrush);
}
