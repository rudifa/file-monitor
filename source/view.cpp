
#include "view.hpp"

#include "utility.hpp"

#include <QWheelEvent>
#include <QGraphicsView>

using namespace utility;

namespace
{
    int const user_invalid_percent = 0;
    int const user_min_percent = 1;
    int const user_max_percent = 100;
}

View::ZoomConfiguration::ZoomConfiguration(Type type, double min_value, double max_value, double initial_value)
    : type(type), min_value(min_value), max_value(max_value), initial_value(initial_value)
{
}

double View::ZoomConfiguration::percentZoomToViewZoom(int percent_zoom) const
{
    if (percent_zoom == user_invalid_percent)
        return initial_value;

    if (type == RampUp)
        return math::rampUp(percent_zoom, user_min_percent, user_max_percent, min_value, max_value);
    else
        return math::scaleValueBetweenRanges(percent_zoom, user_min_percent, user_max_percent, min_value, max_value);
}

// needs work
int View::ZoomConfiguration::viewZoomToPercentZoom(double view_zoom) const
{
    if (type == RampUp)
        return math::inverseRampUp(view_zoom, min_value, max_value, user_min_percent, user_max_percent);
    else
        return math::scaleValueBetweenRanges(view_zoom, min_value, max_value, user_min_percent, user_max_percent);
}

View::View(QObject * parent, ZoomConfiguration zoom_parameters)
    : QObject(parent), zoom_configuration(zoom_parameters), current_percent_zoom(0)
{
    transparent_tile_pixmap = QPixmap(32, 32);
    transparent_tile_pixmap.fill(Qt::white);
    QPainter tile_painter(&transparent_tile_pixmap);
    QColor darker_color(230, 230, 230);
    tile_painter.fillRect(0, 0, 16, 16, darker_color);
    tile_painter.fillRect(16, 16, 16, 16, darker_color);
    tile_painter.end();
}

void View::setPercentageZoom(int percent_zoom)
{
    double view_zoom = zoom_configuration.percentZoomToViewZoom(percent_zoom);
    setZoom(view_zoom);

    current_percent_zoom = zoom_configuration.viewZoomToPercentZoom(view_zoom);
}

int View::getPercentageZoom() const
{
    return current_percent_zoom;
}

void View::resetPercentageZoom()
{
    setPercentageZoom(0);
}

//void View::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "what";

//    if (event->modifiers().testFlag(Qt::ControlModifier))
//    {

//        qDebug() << event->delta();

////        scaleView(pow((double)2, -event->delta() / 240.0));
//    } else {
////        QGraphicsView::wheelEvent(event);
//    }
//}
