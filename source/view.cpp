
#include "view.hpp"

#include "utility.hpp"

#include <QWheelEvent>
#include <QGraphicsView>

using namespace utility;

View::View(ViewScale zoom_parameters, QWidget * parent)
    : QWidget(parent), view_scale(zoom_parameters), current_zoom(0)
{
    transparent_tile_pixmap = QPixmap(32, 32);
    transparent_tile_pixmap.fill(Qt::white);
    QPainter tile_painter(&transparent_tile_pixmap);
    QColor darker_color(230, 230, 230);
    tile_painter.fillRect(0, 0, 16, 16, darker_color);
    tile_painter.fillRect(16, 16, 16, 16, darker_color);
    tile_painter.end();
}

void View::setZoom(int zoom)
{
    double scale = view_scale.zoomToScale(zoom);
    setScale(scale);

    // We can't just use zoom in case we are being handed an invalid zoom (to jump to default).
    slotScaleChanged(scale);
}

int View::getZoom() const
{
    return current_zoom;
}

void View::resetZoom()
{
    setZoom(0);
}

// This only announces that the scale has been changed elsewhere, don't change the scale in here.
void View::slotScaleChanged(double scale)
{
    int zoom = view_scale.scaleToZoom(scale);
    if (zoom != current_zoom)
    {
        current_zoom = zoom;
        signalScaleChanged();
    }
}
