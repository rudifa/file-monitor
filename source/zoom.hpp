
#ifndef ZOOM_HPP
#define ZOOM_HPP

class QWheelEvent;

// Constants for the zoom scroll bar.
namespace zoom
{
    double const invalid = 0;  // Invalid zoom sets view to default scale.
    double const min = 1;
    double const max = 10000;
    double const step = 100;

    int getNumZoomSteps(QWheelEvent * event);

    bool isZoomValid(int zoom);
    bool isZoomChangeValid(int current_zoom, int next_zoom);

    int normalizeZoom(int zoom);
}

// Scaling behavior that needs to be specified for each view.
struct ViewScale
{
    enum Type { Linear, RampUp };

    ViewScale(Type type, double min, double max, double initial);

    double zoomToScale(int zoom) const;
    int scaleToZoom(double scale) const;

    Type type;
    double min;
    double max;
    double initial;
};

#endif
