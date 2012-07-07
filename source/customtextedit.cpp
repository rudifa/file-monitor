
#include "customtextedit.hpp"

#include "ui_mainwindow.h"
#include "contextmenus.hpp"
#include "utility.hpp"

#include <QWheelEvent>
#include <QPointF>
#include <QScrollBar>
#include <QFont>

using namespace utility;

CustomTextEdit::CustomTextEdit(Ui::MainWindow const & ui, ViewScale const & view_scale, QWidget * parent)
    : QTextEdit(parent), ui(ui), view_scale(view_scale)
{
    setReadOnly(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint const &)), SLOT(slotShowContextMenu(QPoint const &)));
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

void CustomTextEdit::slotShowContextMenu(QPoint const & position)
{
    TextContextMenu context_menu(ui);
    context_menu.exec(mapToGlobal(position));
}
