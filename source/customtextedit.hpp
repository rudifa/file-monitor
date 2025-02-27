
#ifndef CUSTOM_TEXT_EDIT_H
#define CUSTOM_TEXT_EDIT_H

#include <QTextEdit>

#include "zoom.hpp"

class QWidget;
class QWheelEvent;

namespace Ui
{
class MainWindow;
}

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT

   public:
    explicit CustomTextEdit(Ui::MainWindow const& ui,
                            ViewScale const& view_scale, QWidget* parent);
    virtual void wheelEvent(QWheelEvent* event);
    void setScale(double scale);

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
