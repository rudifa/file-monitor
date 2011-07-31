
#ifndef SVG_VIEW_HPP
#define SVG_VIEW_HPP

#include "view.hpp"

#include <QGraphicsView>

class QWidget;
class QWheelEvent;
class QPaintEvent;

class SvgView : public QGraphicsView, public View
{
    Q_OBJECT

public:
    SvgView(QWidget * parent);
    ~SvgView();

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setVerticalScroll(int scroll);
    int getVerticalScroll() const;
    void setHorizontalScroll(int scroll);
    int getHorizontalScroll() const;

protected:
    void paintEvent(QPaintEvent *event);

    QGraphicsItem * graphics_item;
};

#endif
