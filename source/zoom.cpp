
#include "zoom.hpp"

#include "utility.hpp"

using namespace utility;

ViewScale::ViewScale(Type type, double min_value, double max_value, double initial_value)
    : type(type), min(min_value), max(max_value), initial(initial_value)
{
}

double ViewScale::zoomToScale(int zoom) const
{
    if (zoom == zoom::invalid)
        return initial;

    if (type == RampUp)
        return math::rampUp(zoom, zoom::min, zoom::max, min, max);
    else
        return math::scaleValueBetweenRanges(zoom, zoom::min, zoom::max, min, max);
}

int ViewScale::scaleToZoom(double scale) const
{
    if (type == RampUp)
        return math::inverseRampUp(scale, min, max, zoom::min, zoom::max);
    else
        return math::scaleValueBetweenRanges(scale, min, max, zoom::min, zoom::max);
}
