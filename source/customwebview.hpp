#ifndef CUSTOM_WEB_VIEW_HPP
#define CUSTOM_WEB_VIEW_HPP

#include <QTimer>
#include <QWebEngineView>

#include "zoom.hpp"

class QWidget;
class QWheelEvent;

namespace Ui
{
class MainWindow;
}

class CustomWebView : public QWebEngineView
{
    Q_OBJECT

   public:
    explicit CustomWebView(Ui::MainWindow const& ui,
                           ViewScale const& view_scale,
                           QWidget* parent = nullptr);
    virtual void wheelEvent(QWheelEvent* event);
    void setScale(double scale);

    bool hasVerticalScrollBar() const;
    bool hasHorizontalScrollBar() const;

   signals:
    void signalScaleChanged(double scale);

   public slots:
    void slotShowContextMenu(QPoint const& position);

   protected:
    virtual void paintEvent(QPaintEvent* event);

   private slots:
    void checkScrollChanges();

   private:
    Ui::MainWindow const& ui;
    ViewScale const& view_scale;
    QTimer scrollCheckTimer;

    double getCurrentScale() const;
};

#endif
