
#include "zoom.hpp"

#include <QWheelEvent>

#include "utility.hpp"

using namespace utility;

int zoom::getNumZoomSteps(QWheelEvent* event)
{
    // Common constants for most mice.
    int const resolver_units_per_degree = 8;
    int const degrees_per_scroll_step = 15;

    // This multiplier allows us to traverse the scroll range at a reasonable
    // speed.
    int const zoom_steps_per_scroll_step = 2;

    int scroll_degrees = event->angleDelta().y() / resolver_units_per_degree;
    int scroll_steps = scroll_degrees / degrees_per_scroll_step;
    return (scroll_steps * zoom_steps_per_scroll_step);
}

bool zoom::isZoomValid(int zoom)
{
    return (zoom >= zoom::min && zoom <= zoom::max);
}

bool zoom::isZoomChangeValid(int current_zoom, int next_zoom)
{
    if (current_zoom == zoom::min && next_zoom <= zoom::min) return false;

    if (current_zoom == zoom::max && next_zoom >= zoom::max) return false;

    return true;
}

int zoom::normalizeZoom(int zoom)
{
    if (zoom < zoom::min) return zoom::min;

    if (zoom > zoom::max) return zoom::max;

    return zoom;
}

ViewScale::ViewScale(Type type, double min_value, double max_value,
                     double initial_value)
    : type(type), min(min_value), max(max_value), initial(initial_value)
{
}

double ViewScale::zoomToScale(int zoom) const
{
    if (zoom == zoom::invalid) return initial;

    if (type == RampUp)
        return math::rampUp(zoom, zoom::min, zoom::max, min, max);
    else
        return math::scaleValueBetweenRanges(zoom, zoom::min, zoom::max, min,
                                             max);
}

int ViewScale::scaleToZoom(double scale) const
{
    if (type == RampUp)
        return math::inverseRampUp(scale, min, max, zoom::min, zoom::max);
    else
        return math::scaleValueBetweenRanges(scale, min, max, zoom::min,
                                             zoom::max);
}
