
#ifndef VIEW_HPP
#define VIEW_HPP

#include <QObject>
#include <QPixmap>

class QString;
class QWidget;
class QPoint;
class QWheelEvent;

class View : public QObject
{
    Q_OBJECT

public:
    struct ZoomConfiguration
    {
        enum Type { Linear, RampUp };

        ZoomConfiguration(Type type, double min_value, double max_value, double initial_value);
        double percentZoomToViewZoom(int percent_zoom) const;
        int viewZoomToPercentZoom(double view_zoom) const;

        Type type;
        double min_value;
        double max_value;
        double initial_value;
    };

    View(QObject * parent, ZoomConfiguration zoom_configuration);

    virtual QWidget * getWidget() = 0;
    virtual bool load(QString const & file_uri) = 0;

    void setPercentageZoom(int percent_zoom);
    int getPercentageZoom() const;
    void resetPercentageZoom();

    virtual void setScrollDimensions(QPoint scroll_dimensions) = 0;
    virtual QPoint getScrollDimensions() const = 0;

    virtual void enableTransparentBackground(bool) { }
    virtual void wordWrap(bool) { }
    virtual void indentXML(bool) { }

//    void wheelEvent(QWheelEvent * event);

signals:
    void signalUserChangedDisplay();
    void signalZoomIn();
    void signalZoomOut();

protected:
    QPixmap transparent_tile_pixmap;

    ZoomConfiguration zoom_configuration;
    int current_percent_zoom;

    virtual void setZoom(double zoom) = 0;
};

#endif
