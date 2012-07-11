
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
    View(ViewScale view_scale, QWidget * parent);

    virtual QWidget * getWidget() = 0;
    virtual bool load(QString const & file_uri, bool is_reload = false) = 0;

    void setZoom(int zoom);
    int getZoom() const;
    void resetZoom();

    virtual void setScrollDimensions(QPoint scroll_dimensions) = 0;
    virtual QPoint getScrollDimensions() const = 0;

    virtual void enableTransparentBackground(bool) { }
    virtual void wordWrap(bool) { }
    virtual void indentXML(bool) { }
    virtual void scrollToBottomOnChange(bool) { }

    virtual void selectAll() { }
    virtual void copy() { }

signals:
    void signalScaleChanged();

public slots:
    virtual void slotFindNext(QString const &, bool) { }
    virtual void slotFindPrevious(QString const &, bool) { }

protected slots:
    void slotScaleChanged(double scale);

protected:
    QPixmap transparent_tile_pixmap;

    ViewScale view_scale;
    int current_zoom;

    virtual void setScale(double zoom) = 0;
};

#endif
