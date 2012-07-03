
#include "imageview.hpp"

#include "customgraphicsview.hpp"
#include "utility.hpp"

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QScrollBar>

#include <cmath>
#include <cassert>

using namespace utility;

ImageView::ImageView(QWidget * parent)
    : View(parent, ViewScale(ViewScale::RampUp, .05, 50, 1)), graphics_view(new CustomGraphicsView(parent, view_scale))
{
}

QWidget * ImageView::getWidget()
{
    QWidget * widget = dynamic_cast<QWidget *>(graphics_view);
    assert(widget);

    return widget;
}

bool ImageView::load(QString const & file_uri)
{
    graphics_view->resetTransform();

    QGraphicsScene * graphics_scene = graphics_view->scene();
    graphics_scene->clear();

    graphics_item = new QGraphicsPixmapItem(QPixmap(file_uri));
    graphics_item->setFlags(QGraphicsItem::ItemClipsToShape);
    graphics_item->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    graphics_item->setTransformationMode(Qt::FastTransformation);
    graphics_item->setZValue(0);

    graphics_scene->addItem(graphics_item);

    connect(graphics_view, SIGNAL(signalScaleChanged(double)), SLOT(slotScaleChanged(double)));

//    // TODO: Figure out why there is an extra 10 pixels (varies with zoom level)
//    //  to the right and bottom of QPixmap inside QGraphicsScene and remove it.
//    // --- This is a Linux issue (KDE for sure, I haven't checked GNOME).
//    {
//        QRectF graphics_item_size = graphics_item->boundingRect();
//        graphics_item_size.adjust(0, 0, -13, -13);
//        graphics_scene->setSceneRect(graphics_item_size);
//    }

    return true;
}

void ImageView::setScale(double scale)
{
    graphics_view->resetTransform();
    graphics_view->scale(scale, scale);
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
