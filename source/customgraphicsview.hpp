
#ifndef CUSTOM_GRAPHICS_VIEW_H
#define CUSTOM_GRAPHICS_VIEW_H

#include "zoom.hpp"

#include <QGraphicsView>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CustomGraphicsView(QWidget * parent, ViewScale const & view_scale);
    virtual void wheelEvent(QWheelEvent * event);

signals:
    void signalScaleChanged(double scale);

private:
    ViewScale const & view_scale;
    double getCurrentScale() const;
};

#endif
