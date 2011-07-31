
#include "imageview.hpp"

#include <QLabel>
#include <QImage>
#include <QScrollArea>
#include <QScrollBar>

#include <cmath>
#include <cassert>

ImageView::ImageView(QWidget * parent)
    : View(ZoomParameters(.04, .2, 10)), image_label(new QLabel(parent)), scroll_area(new QScrollArea(parent))
{
    image_label->setBackgroundRole(QPalette::Base);
    image_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    image_label->setScaledContents(true);

    scroll_area->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    scroll_area->setBackgroundRole(QPalette::Light);
    scroll_area->setWidget(image_label);
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

void ImageView::setVerticalScroll(int scroll)
{
    scroll_area->verticalScrollBar()->setSliderPosition(scroll);
}

int ImageView::getVerticalScroll() const
{
    return scroll_area->verticalScrollBar()->value();
}

void ImageView::setHorizontalScroll(int scroll)
{
    scroll_area->horizontalScrollBar()->setValue(scroll);
}

int ImageView::getHorizontalScroll() const
{
    return scroll_area->horizontalScrollBar()->value();
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
