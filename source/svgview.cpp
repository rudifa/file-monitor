
#include "svgview.hpp"

#include <QScrollBar>
#include <QWheelEvent>
#include <QtSvg>
#include <QtSvgWidgets/QGraphicsSvgItem>
#include <cassert>
#include <cmath>

#include "customgraphicsview.hpp"
#include "tabpage.hpp"
#include "utility.hpp"

using namespace utility;

SvgView::SvgView(Ui::MainWindow const &ui, QWidget *parent)
    : View(ViewScale(ViewScale::RampUp, .1, 500, 1), parent),
      graphics_view(new CustomGraphicsView(ui, view_scale, parent))
{
}

QWidget *SvgView::getWidget()
{
    QWidget *widget = dynamic_cast<QWidget *>(graphics_view);
    assert(widget);

    return widget;
}

bool SvgView::load(QString const &file_uri, bool)
{
    QGraphicsScene *graphics_scene = graphics_view->scene();

    graphics_scene->clear();
    graphics_view->resetTransform();

    graphics_item = new QGraphicsSvgItem(file_uri);
    graphics_item->setFlags(QGraphicsItem::ItemClipsToShape);
    graphics_item->setCacheMode(QGraphicsItem::NoCache);
    graphics_item->setZValue(0);

    graphics_scene->addItem(graphics_item);

    connect(graphics_view, SIGNAL(signalScaleChanged(double)),
            SLOT(slotScaleChanged(double)));

    return true;
}

void SvgView::setScale(double scale)
{
    graphics_view->resetTransform();
    graphics_view->scale(scale, scale);
}

void SvgView::setScrollDimensions(QPoint dimensions)
{
    graphics_view->horizontalScrollBar()->setValue(dimensions.x());
    graphics_view->verticalScrollBar()->setValue(dimensions.y());
}

QPoint SvgView::getScrollDimensions() const
{
    int horizontal_scroll = graphics_view->horizontalScrollBar()->value();
    int vertical_scroll = graphics_view->verticalScrollBar()->value();
    return QPoint(horizontal_scroll, vertical_scroll);
}

void SvgView::makeBackgroundTransparent(bool transparent)
{
    graphics_view->setBackgroundBrush(
        transparent ? QBrush(transparent_tile_pixmap) : Qt::NoBrush);
}
