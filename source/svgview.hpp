
#ifndef SVG_VIEW_HPP
#define SVG_VIEW_HPP

#include "view.hpp"

class QWidget;
class QGraphicsView;
class QGraphicsItem;

class SvgView : public View
{
public:
    SvgView(QWidget * parent);
    ~SvgView();

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

protected:
    QGraphicsView * graphics_view;
    QGraphicsItem * graphics_item;
};

#endif
