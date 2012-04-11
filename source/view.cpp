
#include "view.hpp"

#include <QWheelEvent>
#include <QGraphicsView>

#include <QDebug>

View::View(QObject * parent, ZoomParameters zoom_parameters)
    : QObject(parent), zoom_parameters(zoom_parameters)
{
}

double View::getZoom() const
{
    return absolute_zoom;
}

void View::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {

        qDebug() << event->delta();

//        scaleView(pow((double)2, -event->delta() / 240.0));
    } else {
//        QGraphicsView::wheelEvent(event);
    }
}
