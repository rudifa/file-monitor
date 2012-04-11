
#ifndef CUSTOM_GRAPHICS_VIEW_H
#define CUSTOM_GRAPHICS_VIEW_H

#include <QGraphicsView>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CustomGraphicsView(QWidget * parent = 0);
    void wheelEvent(QWheelEvent * event);

signals:
    void signalZoomIn();
    void signalZoomOut();
};

#endif
