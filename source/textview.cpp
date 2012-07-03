
#include "textview.hpp"

#include "zoom.hpp"
#include "utility.hpp"

#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDomDocument>

#include <cmath>
#include <cassert>

using namespace utility;

TextView::TextView(QWidget * parent)
    : View(parent, ViewScale(ViewScale::Linear, zoom::min, zoom::max, 12)),
      text_edit(new QTextEdit(parent)), indent_xml(false)
{
    text_edit->setReadOnly(true);
}

QWidget * TextView::getWidget()
{
    QWidget * widget = dynamic_cast<QWidget *>(text_edit);
    assert(widget);

    return widget;
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

void TextView::setScale(double scale)
{
    QFont current_font = text_edit->font();
    current_font.setPixelSize(math::round(scale));
    text_edit->setFont(current_font);
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
