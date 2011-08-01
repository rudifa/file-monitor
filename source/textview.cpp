
#include "textview.hpp"

#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QTimer>

#include <cmath>
#include <cassert>

TextView::TextView(QWidget * parent)
    : QObject(parent), View(ZoomParameters(.5, 8, 12)), text_edit(new QTextEdit(parent))
{
    text_edit->setReadOnly(true);
}

QWidget * TextView::getWidget()
{
    return text_edit;
}

bool TextView::load(QString const & file_uri)
{
    QFile file(file_uri);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString file_str = in.readAll();

    text_edit->setText(file_str);
    return true;
}

void TextView::setZoom(double zoom)
{
    if (zoom == 0.0)
        zoom = zoom_parameters.initial;

    double relative_zoom = zoom * zoom_parameters.scale + zoom_parameters.offset;

    QFont current_font = text_edit->font();
    current_font.setPixelSize(relative_zoom);
    text_edit->setFont(current_font);

    absolute_zoom = zoom;
}

void TextView::setScrollDimensions(QPoint dimensions)
{
    scroll_dimensions = dimensions;
    slotSetScroll();
}

QPoint TextView::getScrollDimensions() const
{
    int horizontal_scroll = text_edit->horizontalScrollBar()->value();
    int vertical_scroll = text_edit->verticalScrollBar()->value();
    return QPoint(horizontal_scroll, vertical_scroll);
}

//  Periodically check to see if the control is ready for the scroll to be set.
void TextView::slotSetScroll()
{
    int max_scroll_vertical = text_edit->verticalScrollBar()->maximum();
    int max_scroll_horizontal = text_edit->horizontalScrollBar()->maximum();
    QPoint scroll_max(max_scroll_horizontal, max_scroll_vertical);

    if (scroll_max.x() >= scroll_dimensions.x() && scroll_max.y() >= scroll_dimensions.y())
    {
        text_edit->horizontalScrollBar()->setSliderPosition(scroll_dimensions.x());
        text_edit->verticalScrollBar()->setSliderPosition(scroll_dimensions.y());
    } else {
        QTimer::singleShot(10, this, SLOT(slotSetScroll()));
    }
}
