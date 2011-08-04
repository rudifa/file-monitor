
#include "textview.hpp"

#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QTimer>

#include <cmath>
#include <cassert>

TextView::TextView(QWidget * parent)
    : View(parent, ZoomParameters(.5, 8, 12)), text_edit(new QTextEdit(parent))
{
    text_edit->setReadOnly(true);

    connect(text_edit->verticalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
    connect(text_edit->horizontalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
}

TextView::~TextView()
{

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
    text_edit->horizontalScrollBar()->setSliderPosition(dimensions.x());
    text_edit->verticalScrollBar()->setSliderPosition(dimensions.y());
}

QPoint TextView::getScrollDimensions() const
{
    int horizontal_scroll = text_edit->horizontalScrollBar()->value();
    int vertical_scroll = text_edit->verticalScrollBar()->value();
    return QPoint(horizontal_scroll, vertical_scroll);
}
