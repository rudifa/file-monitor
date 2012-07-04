
#ifndef CUSTOM_WEB_VIEW_HPP
#define CUSTOM_WEB_VIEW_HPP

#include "zoom.hpp"

#include <QWebView>

class QWidget;
class QWheelEvent;

class CustomWebView : public QWebView
{
    Q_OBJECT

public:
    explicit CustomWebView(QWidget * parent, ViewScale const & view_scale);
    virtual void wheelEvent(QWheelEvent * event);
    void setScale(double scale);

signals:
    void signalScaleChanged(double scale);

private:
    ViewScale const & view_scale;

    double getCurrentScale() const;
};

#endif


