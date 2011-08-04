
#include "view.hpp"

View::View(QObject * parent, ZoomParameters zoom_parameters)
    : QObject(parent), zoom_parameters(zoom_parameters)
{
}

double View::getZoom() const
{
    return absolute_zoom;
}
