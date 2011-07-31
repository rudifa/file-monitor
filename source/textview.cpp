
#include "textview.hpp"

#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>

#include <cmath>
#include <cassert>

TextView::TextView(QWidget * parent)
    : View(ZoomParameters(.5, 8, 12)), text_edit(new QTextEdit(parent))
{
    text_edit->setReadOnly(true);
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

void TextView::setVerticalScroll(int scroll)
{
    text_edit->verticalScrollBar()->setSliderPosition(scroll);
}

int TextView::getVerticalScroll() const
{
    return text_edit->verticalScrollBar()->value();
}

void TextView::setHorizontalScroll(int scroll)
{
    text_edit->horizontalScrollBar()->setValue(scroll);
}

int TextView::getHorizontalScroll() const
{
    return text_edit->horizontalScrollBar()->value();
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
