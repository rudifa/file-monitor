
#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "view.hpp"

class QWidget;
class QGraphicsView;
class QGraphicsPixmapItem;

class ImageView : public View
{
public:
    ImageView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void enableTransparentBackground(bool enable);

protected:
    QGraphicsView * graphics_view;
    QGraphicsPixmapItem * graphics_item;
};

#endif
