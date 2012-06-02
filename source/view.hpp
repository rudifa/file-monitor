
#ifndef VIEW_HPP
#define VIEW_HPP

#include <QObject>
#include <QPixmap>

class QString;
class QWidget;
class QPoint;
class QWheelEvent;

struct ZoomParameters
{
    ZoomParameters(double scale, double offset, double initial)
        : scale(scale), offset(offset), initial(initial) { }
    double scale;
    double offset;
    double initial;
};

class View : public QObject
{
    Q_OBJECT

public:
    View(QObject * parent, ZoomParameters zoom_parameters);

    virtual QWidget * getWidget() = 0;
    virtual bool load(QString const & file_uri) = 0;

    virtual void setZoom(double zoom) = 0;
    double getZoom() const;

    virtual void setScrollDimensions(QPoint scroll_dimensions) = 0;
    virtual QPoint getScrollDimensions() const = 0;

    virtual void enableTransparentBackground(bool enable) { }

//    void wheelEvent(QWheelEvent * event);

signals:
    void signalUserChangedDisplay();
    void signalZoomIn();
    void signalZoomOut();

protected:
    ZoomParameters zoom_parameters;
    double absolute_zoom;
    QPixmap transparent_tile_pixmap;
};

#endif
