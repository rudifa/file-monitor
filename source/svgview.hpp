
#ifndef SVG_VIEW_HPP
#define SVG_VIEW_HPP

#include "view.hpp"

class QWidget;
class QGraphicsView;
class QGraphicsItem;
class QWheelEvent;

class CustomGraphicsView;

class SvgView : public View
{
public:
    SvgView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setScale(double scale);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void enableTransparentBackground(bool enable);

protected:
    CustomGraphicsView * graphics_view;
    QGraphicsItem * graphics_item;
};

#endif
