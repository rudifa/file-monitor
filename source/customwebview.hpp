
#ifndef CUSTOM_WEB_VIEW_HPP
#define CUSTOM_WEB_VIEW_HPP

#include "zoom.hpp"

#include <QWebView>

class QWidget;
class QWheelEvent;

namespace Ui { class MainWindow; }

class CustomWebView : public QWebView
{
    Q_OBJECT

public:
    explicit CustomWebView(Ui::MainWindow const & ui, ViewScale const & view_scale, QWidget * parent);
    virtual void wheelEvent(QWheelEvent * event);
    void setScale(double scale);

signals:
    void signalScaleChanged(double scale);

public slots:
    void slotShowContextMenu(QPoint const & position);

private:
    Ui::MainWindow const & ui;
    ViewScale const & view_scale;

    double getCurrentScale() const;
};

#endif


