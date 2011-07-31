
#include "view.hpp"

View::View(ZoomParameters zoom_parameters)
    : zoom_parameters(zoom_parameters)
{
}

View::~View()
{
}

double View::getZoom() const
{
    return absolute_zoom;
}
