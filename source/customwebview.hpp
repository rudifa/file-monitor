
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

protected:
    // Create our own border around the page content.
    virtual void paintEvent(QPaintEvent * event);

private slots:
    // The border we draw will not be updated during scroll unless we do it manually.
    void slotPageScrollRequested(int delta_x, int delta_y, QRect);

private:
    Ui::MainWindow const & ui;
    ViewScale const & view_scale;

    double getCurrentScale() const;
};

#endif


