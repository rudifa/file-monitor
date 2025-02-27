
#include "customwebview.hpp"

#include <QAction>
#include <QMenu>
#include <QPainter>
#include <QPointF>
#include <QWebEnginePage>
#include <QWheelEvent>

#include "contextmenus.hpp"
#include "ui_mainwindow.h"
#include "utility.hpp"

using namespace utility;

namespace
{
int const border_width = 1;
int const border_margin_width = 3;
}  // namespace

CustomWebView::CustomWebView(Ui::MainWindow const &ui,
                             ViewScale const &view_scale, QWidget *parent)
    : QWebEngineView(parent), ui(ui), view_scale(view_scale)
{
    setAcceptDrops(false);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint const &)),
            SLOT(slotShowContextMenu(QPoint const &)));

    // Set up timer to check for scroll changes
    scrollCheckTimer.setInterval(100);  // Check every 100ms
    connect(&scrollCheckTimer, &QTimer::timeout, this,
            &CustomWebView::checkScrollChanges);
    scrollCheckTimer.start();
}

void CustomWebView::wheelEvent(QWheelEvent *event)
{
    // ... (keep the existing implementation)
}

void CustomWebView::setScale(double scale)
{
    // ... (keep the existing implementation)
}

void CustomWebView::slotShowContextMenu(QPoint const &position)
{
    // ... (keep the existing implementation)
}

void CustomWebView::paintEvent(QPaintEvent *event)
{
    QWebEngineView::paintEvent(event);

    bool hasVertical = hasVerticalScrollBar();
    bool hasHorizontal = hasHorizontalScrollBar();

    int max_x = hasVertical ? width() - border_margin_width : width();
    int max_y = hasHorizontal ? height() - border_margin_width : height();

    // Draw the border.
    QPainter painter(this);
    painter.setPen(palette().mid().color());
    painter.drawRect(QRect(QPoint(0, 0), QSize(max_x, max_y))
                         .adjusted(0, 0, -border_width, -border_width));

    // Draw the border margin if there is a scrollbar.
    if (hasVertical)
    {
        int margin_height =
            hasHorizontal ? height() - border_margin_width : height();
        painter.fillRect(
            QRect(QPoint(max_x, 0), QSize(border_margin_width, margin_height)),
            palette().window().color());
    }
    if (hasHorizontal)
    {
        int margin_width =
            hasVertical ? width() - border_margin_width : width();
        painter.fillRect(
            QRect(QPoint(0, max_y), QSize(margin_width, border_margin_width)),
            palette().window().color());
    }
}

void CustomWebView::checkScrollChanges()
{
    // This method is called periodically to check for scroll changes
    update();
}

double CustomWebView::getCurrentScale() const { return page()->zoomFactor(); }

bool CustomWebView::hasVerticalScrollBar() const
{
    QSize contentsSize = page()->contentsSize().toSize();
    QSize viewportSize = size();
    return contentsSize.height() > viewportSize.height();
}

bool CustomWebView::hasHorizontalScrollBar() const
{
    QSize contentsSize = page()->contentsSize().toSize();
    QSize viewportSize = size();
    return contentsSize.width() > viewportSize.width();
}
