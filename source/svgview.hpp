
#ifndef SVG_VIEW_HPP
#define SVG_VIEW_HPP

#include "view.hpp"

class QWidget;
class QGraphicsView;
class QGraphicsItem;
class QWheelEvent;

class CustomGraphicsView;

namespace Ui { class MainWindow; }

class SvgView : public View
{
public:
    SvgView(Ui::MainWindow const & ui, QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri, bool is_reload = false);

    void setScale(double scale);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void enableTransparentBackground(bool enable);

protected:
    CustomGraphicsView * graphics_view;
    QGraphicsItem * graphics_item;
};

#endif
