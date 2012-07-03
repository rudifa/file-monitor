
#ifndef VIEW_HPP
#define VIEW_HPP

#include "zoom.hpp"

#include <QWidget>
#include <QPixmap>

class QString;
class QPoint;
class QWheelEvent;

class View : public QWidget
{
    Q_OBJECT

public:
    View(QWidget * parent, ViewScale view_scale);

    virtual QWidget * getWidget() = 0;
    virtual bool load(QString const & file_uri) = 0;

    void setZoom(int zoom);
    int getZoom() const;
    void resetZoom();

    virtual void setScrollDimensions(QPoint scroll_dimensions) = 0;
    virtual QPoint getScrollDimensions() const = 0;

    virtual void enableTransparentBackground(bool) { }
    virtual void wordWrap(bool) { }
    virtual void indentXML(bool) { }

signals:
    void signalScaleChanged();

protected slots:
    void slotScaleChanged(double scale);

protected:
    QPixmap transparent_tile_pixmap;

    ViewScale view_scale;
    int current_zoom;

    virtual void setScale(double zoom) = 0;
};

#endif
