
#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "view.hpp"

class QWidget;
class QGraphicsView;
class QGraphicsPixmapItem;

class CustomGraphicsView;

namespace Ui { class MainWindow; }

class ImageView : public View
{
public:
    ImageView(Ui::MainWindow const & ui, QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri, bool is_reload = false);

    void setScale(double zoom);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void makeBackgroundTransparent(bool transparent);

protected:
    CustomGraphicsView * graphics_view;
    QGraphicsPixmapItem * graphics_item;
};

#endif
