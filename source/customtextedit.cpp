
#include "customtextedit.hpp"

#include "utility.hpp"

#include <QWheelEvent>
#include <QPointF>
#include <QScrollBar>
#include <QFont>

using namespace utility;

CustomTextEdit::CustomTextEdit(QWidget * parent, ViewScale const & view_scale)
    : QTextEdit(parent), view_scale(view_scale)
{
    setReadOnly(true);
}

void CustomTextEdit::wheelEvent(QWheelEvent * event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        int zoom_steps = zoom::getNumZoomSteps(event);
        if (zoom_steps == 0)
        {
            event->ignore();
            return;
        }

        auto current_zoom = view_scale.scaleToZoom(getCurrentScale());
        auto new_zoom = (current_zoom + (zoom_steps * zoom::step));

        // Ensure we haven't stepped out of the zoom range.
        {
            if (current_zoom == zoom::min && new_zoom <= zoom::min)
                return;
            if (current_zoom == zoom::max && new_zoom >= zoom::max)
                return;

            if (new_zoom < zoom::min)
                new_zoom = zoom::min;
            if (new_zoom > zoom::max)
                new_zoom = zoom::max;
        }

        auto new_scale = view_scale.zoomToScale(new_zoom);
        setScale(new_scale);
        event->accept();
        signalScaleChanged(new_scale);
    }
    else
    {
        QTextEdit::wheelEvent(event);
    }
}

void CustomTextEdit::setScale(double scale)
{
    QFont current_font = font();
    current_font.setPixelSize(math::round(scale));
    setFont(current_font);
}

double CustomTextEdit::getCurrentScale() const
{
    QFont current_font = font();
    return current_font.pixelSize();
}
