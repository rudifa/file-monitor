
#include "view.hpp"

View::View(ZoomParameters zoom_parameters)
    : zoom_parameters(zoom_parameters)
{
}

double View::getZoom() const
{
    return absolute_zoom;
}
