
#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "view.hpp"

class QWidget;
class QGraphicsView;
class QGraphicsPixmapItem;

class CustomGraphicsView;

class ImageView : public View
{
public:
    ImageView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setScale(double zoom);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void enableTransparentBackground(bool enable);

protected:
    CustomGraphicsView * graphics_view;
    QGraphicsPixmapItem * graphics_item;
};

#endif
