
#include "textview.hpp"

#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDomDocument>

#include <cmath>
#include <cassert>

TextView::TextView(QWidget * parent)
    : View(parent, ZoomParameters(.5, 8, 12)), text_edit(new QTextEdit(parent)), indent_xml(false)
{
    text_edit->setReadOnly(true);

    connect(text_edit->verticalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
    connect(text_edit->horizontalScrollBar(), SIGNAL(valueChanged(int)), SIGNAL(signalUserChangedDisplay()));
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
    unformatted_content = in.readAll();

    formatAndInsertContent(unformatted_content, indent_xml);

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

void TextView::wordWrap(bool word_wrap)
{
    auto word_wrap_mode = (word_wrap ? QTextOption::WordWrap : QTextOption::NoWrap);
    text_edit->setWordWrapMode(word_wrap_mode);
}

void TextView::indentXML(bool indent)
{
    indent_xml = indent;
    formatAndInsertContent(unformatted_content, indent_xml);
}

void TextView::formatAndInsertContent(QString const & content, bool indent)
{
    QDomDocument document;
    if (indent && document.setContent(content))
        text_edit->setPlainText(document.toString(4));
    else
        text_edit->setPlainText(content);
}
