
#include "customgraphicsview.hpp"

#include "ui_mainwindow.h"
#include "contextmenus.hpp"
#include "utility.hpp"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>

using namespace utility;

CustomGraphicsView::CustomGraphicsView(Ui::MainWindow const & ui, ViewScale const & view_scale, QWidget * parent)
    : QGraphicsView(parent), ui(ui), view_scale(view_scale)
{
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);

    setScene(new QGraphicsScene(this));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint const &)), SLOT(slotShowContextMenu(QPoint const &)));
}

void CustomGraphicsView::wheelEvent(QWheelEvent * event)
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
            if (!zoom::isZoomChangeValid(current_zoom, new_zoom))
                return;

            new_zoom = zoom::normalizeZoom(new_zoom);
        }

        auto new_scale = view_scale.zoomToScale(new_zoom);

        auto mouse_position_before = mapToScene(event->pos());

        // Scale the view.
        resetTransform();
        scale(new_scale, new_scale);

        // Record how many pixels the mouse has moved (we want to move it back).
        auto mouse_position_after = mapToScene(event->pos());
        auto mouse_delta = mouse_position_before - mouse_position_after;

        auto slider_position = QPointF(horizontalScrollBar()->value(), verticalScrollBar()->value());
        auto delta = mouse_delta * getCurrentScale();

        int after_slider_x_pos = slider_position.x() + std::round(delta.x());
        int after_slider_y_pos = slider_position.y() + std::round(delta.y());

        // Move the pixel that was under the mouse before the scale back under the mouse.
        horizontalScrollBar()->setValue(after_slider_x_pos);
        verticalScrollBar()->setValue(after_slider_y_pos);

        event->accept();

        signalScaleChanged(new_scale);
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

double CustomGraphicsView::getCurrentScale() const
{
    // Because we are using a 1:1 ratio, the scale of either dimension can be used.
    return transform().m11();
}

void CustomGraphicsView::slotShowContextMenu(QPoint const & position)
{
    ImageContextMenu context_menu(ui);
    context_menu.exec(mapToGlobal(position));
}
