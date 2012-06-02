
#include "view.hpp"

#include <QWheelEvent>
#include <QGraphicsView>

#include <QDebug>

View::View(QObject * parent, ZoomParameters zoom_parameters)
    : QObject(parent), zoom_parameters(zoom_parameters)
{
    transparent_tile_pixmap = QPixmap(32, 32);
    transparent_tile_pixmap.fill(Qt::white);
    QPainter tile_painter(&transparent_tile_pixmap);
    QColor darker_color(230, 230, 230);
    tile_painter.fillRect(0, 0, 16, 16, darker_color);
    tile_painter.fillRect(16, 16, 16, 16, darker_color);
    tile_painter.end();
}

double View::getZoom() const
{
    return absolute_zoom;
}

//void View::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "what";

//    if (event->modifiers().testFlag(Qt::ControlModifier))
//    {

//        qDebug() << event->delta();

////        scaleView(pow((double)2, -event->delta() / 240.0));
//    } else {
////        QGraphicsView::wheelEvent(event);
//    }
//}
