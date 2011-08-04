
#include "imageview.hpp"

#include <QLabel>
#include <QImage>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>

#include <cmath>
#include <cassert>

ImageView::ImageView(QWidget * parent)
    : View(parent, ZoomParameters(.04, .2, 20)),
    image_label(new QLabel(parent)), scroll_area(new QScrollArea(parent))
{
    image_label->setBackgroundRole(QPalette::Base);
    image_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    image_label->setScaledContents(true);

    scroll_area->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    scroll_area->setBackgroundRole(QPalette::Light);
    scroll_area->setWidget(image_label);

    connect(scroll_area->verticalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
    connect(scroll_area->horizontalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
}

ImageView::~ImageView()
{
}

QWidget * ImageView::getWidget()
{
    return scroll_area;
}

bool ImageView::load(QString const & file_uri)
{
    QImage image(file_uri);
    if (image.isNull())
        return false;

    image_label->setPixmap(QPixmap::fromImage(image));
    return true;
}

void ImageView::setZoom(double zoom)
{
    if (zoom == 0.0)
        zoom = zoom_parameters.initial;

    double relative_zoom = zoom * zoom_parameters.scale + zoom_parameters.offset;

    assert(image_label->pixmap());
    image_label->resize(relative_zoom * image_label->pixmap()->size());

    absolute_zoom = zoom;
}

void ImageView::setScrollDimensions(QPoint dimensions)
{
    scroll_area->verticalScrollBar()->setSliderPosition(dimensions.y());
    scroll_area->horizontalScrollBar()->setSliderPosition(dimensions.x());
}

QPoint ImageView::getScrollDimensions() const
{
    int horizontal_scroll = scroll_area->horizontalScrollBar()->value();
    int vertical_scroll = scroll_area->verticalScrollBar()->value();
    return QPoint(horizontal_scroll, vertical_scroll);
}
