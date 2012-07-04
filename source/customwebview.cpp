
#include "customwebview.hpp"

#include "utility.hpp"

#include <QWheelEvent>
#include <QPointF>
#include <QScrollBar>
#include <QWebPage>
#include <QWebFrame>

using namespace utility;

CustomWebView::CustomWebView(QWidget * parent, ViewScale const & view_scale)
    : QWebView(parent), view_scale(view_scale)
{
    setAcceptDrops(false);
}

// TODO: This is a duplicate of CustomTextEdit::wheelEvent and shares a ton
//  with CustomGraphicsView::wheelEvent.  Reduce this shared functionality.
void CustomWebView::wheelEvent(QWheelEvent * event)
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
        QWebView::wheelEvent(event);
    }
}

void CustomWebView::setScale(double scale)
{
    page()->currentFrame()->setZoomFactor(scale);
}

double CustomWebView::getCurrentScale() const
{
    return page()->currentFrame()->zoomFactor();
}
