
#ifndef CUSTOM_GRAPHICS_VIEW_H
#define CUSTOM_GRAPHICS_VIEW_H

#include <QGraphicsView>

#include "zoom.hpp"

namespace Ui
{
class MainWindow;
}

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

   public:
    explicit CustomGraphicsView(Ui::MainWindow const& ui,
                                ViewScale const& view_scale, QWidget* parent);
    virtual void wheelEvent(QWheelEvent* event);

   signals:
    void signalScaleChanged(double scale);

   public slots:
    void slotShowContextMenu(QPoint const& position);

   private:
    Ui::MainWindow const& ui;
    ViewScale const& view_scale;

    double getCurrentScale() const;
};

#endif
