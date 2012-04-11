
#include "customgraphicsview.hpp"

#include <QWheelEvent>

CustomGraphicsView::CustomGraphicsView(QWidget * parent)
    : QGraphicsView(parent)
{
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        if (event->delta() > 0)
            signalZoomIn();
        else
            signalZoomOut();
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}
