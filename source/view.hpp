
#ifndef VIEW_HPP
#define VIEW_HPP

class QString;
class QWidget;

struct ZoomParameters
{
    ZoomParameters(double scale, double offset, double initial)
        : scale(scale), offset(offset), initial(initial) { }
    double scale;
    double offset;
    double initial;
};

class View
{
public:
    View(ZoomParameters zoom_parameters);
    virtual ~View();

    virtual QWidget * getWidget() = 0;
    virtual bool load(QString const & file_uri) = 0;

    virtual void setZoom(double zoom) = 0;
    double getZoom() const;

    virtual void setVerticalScroll(int scroll) = 0;
    virtual int getVerticalScroll() const = 0;
    virtual void setHorizontalScroll(int scroll) = 0;
    virtual int getHorizontalScroll() const = 0;

protected:
    double absolute_zoom;

    ZoomParameters zoom_parameters;
};

#endif
