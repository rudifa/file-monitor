
#ifndef CUSTOM_TEXT_EDIT_H
#define CUSTOM_TEXT_EDIT_H

#include "zoom.hpp"

#include <QTextEdit>

class QWidget;
class QWheelEvent;

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit CustomTextEdit(QWidget * parent, ViewScale const & view_scale);
    virtual void wheelEvent(QWheelEvent * event);
    void setScale(double scale);
    
signals:
    void signalScaleChanged(double scale);

private:
    ViewScale const & view_scale;

    double getCurrentScale() const;
};

#endif
