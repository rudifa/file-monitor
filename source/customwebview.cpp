
#include "customwebview.hpp"

#include "ui_mainwindow.h"
#include "contextmenus.hpp"
#include "utility.hpp"

#include <QWheelEvent>
#include <QPointF>
#include <QScrollBar>
#include <QWebPage>
#include <QWebFrame>
#include <QMenu>
#include <QAction>

using namespace utility;

namespace
{
    int const border_width = 1;
    int const border_margin_width = 3;
}

CustomWebView::CustomWebView(Ui::MainWindow const & ui, ViewScale const & view_scale, QWidget * parent)
    : QWebView(parent), ui(ui), view_scale(view_scale)
{
    setAcceptDrops(false);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint const &)), SLOT(slotShowContextMenu(QPoint const &)));

    connect(page(), SIGNAL(scrollRequested(int,int,QRect)), SLOT(slotPageScrollRequested(int,int,QRect)));
}

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
            if (!zoom::isZoomChangeValid(current_zoom, new_zoom))
                return;

            new_zoom = zoom::normalizeZoom(new_zoom);
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

void CustomWebView::slotShowContextMenu(QPoint const & position)
{
    TextContextMenu context_menu(ui);
    context_menu.exec(mapToGlobal(position));
}

void CustomWebView::paintEvent(QPaintEvent * event)
{
    QWebView::paintEvent(event);

    QRect vertical_scroll_geometry = page()->mainFrame()->scrollBarGeometry(Qt::Vertical);
    QRect horizontal_scroll_geometry = page()->mainFrame()->scrollBarGeometry(Qt::Horizontal);

    int max_x = vertical_scroll_geometry.isEmpty() ? width() : width() - vertical_scroll_geometry.width() - border_margin_width;
    int max_y = horizontal_scroll_geometry.isEmpty() ? height() : height() - horizontal_scroll_geometry.height() - border_margin_width;

    // Draw the border.
    QPainter painter(this);
    painter.setPen(palette().mid().color());
    painter.drawRect(QRect(QPoint(0, 0), QSize(max_x, max_y)).adjusted(0, 0, -border_width, -border_width));

    // Draw the border margin if there is a scrollbar.
    if (!vertical_scroll_geometry.isEmpty())
    {
        int margin_height = horizontal_scroll_geometry.isEmpty() ? height() : height() - horizontal_scroll_geometry.height();
        painter.fillRect(QRect(QPoint(max_x, 0), QSize(border_margin_width, margin_height)), palette().background().color());
    }
    if (!horizontal_scroll_geometry.isEmpty())
    {
        int margin_width = vertical_scroll_geometry.isEmpty() ? width() : width() - vertical_scroll_geometry.width();
        painter.fillRect(QRect(QPoint(0, max_y), QSize(margin_width, border_margin_width)), palette().background().color());
    }
}

// The border we draw will not be updated during scroll unless we do it manually.
void CustomWebView::slotPageScrollRequested(int delta_x, int delta_y, QRect)
{
    QRect vertical_scroll_geometry = page()->mainFrame()->scrollBarGeometry(Qt::Vertical);
    QRect horizontal_scroll_geometry = page()->mainFrame()->scrollBarGeometry(Qt::Horizontal);

    int vertical_padding = vertical_scroll_geometry.isEmpty() ? 0 : border_margin_width;
    int horizontal_padding = horizontal_scroll_geometry.isEmpty() ? 0 : border_margin_width;

    int max_x = vertical_scroll_geometry.isEmpty() ? width() : width() - vertical_scroll_geometry.width() - border_margin_width;
    int max_y = horizontal_scroll_geometry.isEmpty() ? height() : height() - horizontal_scroll_geometry.height() - border_margin_width;

    // Vertical scrolling.
    if (delta_y > 0)
    {
        // Erase the old drawn bottom border.
        update(QRect(QPoint(0, delta_y), QSize(max_x, border_width)));
        update(QRect(QPoint(0, max_y - border_width), QSize(max_x, vertical_padding + border_width)));  // Redraw the bottom border & padding.
    }
    else if (delta_y < 0)
    {
        // Erase the old drawn top border.
        update(QRect(QPoint(0, max_y + delta_y - border_width), QSize(max_x, vertical_padding + border_width)));
        update(QRect(QPoint(0, 0), QSize(max_x, border_width)));  // Redraw the top border
    }

    // Horizontal scrolling.
    if (delta_x > 0)
    {
        // Erase the old drawn left border.
        update(QRect(QPoint(delta_x, 0), QSize(border_width, max_y)));
        update(QRect(QPoint(max_x - border_width, 0), QSize(horizontal_padding + border_width, max_y)));  // Redraw the right border & padding.
    }
    else if (delta_x < 0)
    {
        // Erase the old drawn right border.
        update(QRect(QPoint(max_x + delta_x - border_width, 0), QSize(horizontal_padding + border_width, max_y)));
        update(QRect(QPoint(0, 0), QSize(border_width, max_y)));  // Redraw the left border
    }
}

double CustomWebView::getCurrentScale() const
{
    return page()->currentFrame()->zoomFactor();
}
