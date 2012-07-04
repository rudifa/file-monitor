
#include "textview.hpp"

#include "customtextedit.hpp"
#include "zoom.hpp"
#include "utility.hpp"

#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDomDocument>
#include <QWheelEvent>

#include <cmath>
#include <cassert>

using namespace utility;

TextView::TextView(QWidget * parent)
    : View(parent, ViewScale(ViewScale::Linear, 1, 100, 12)),
      text_edit(new CustomTextEdit(parent, view_scale)), indent_xml(false)
{
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

    connect(text_edit, SIGNAL(signalScaleChanged(double)), SLOT(slotScaleChanged(double)));

    return true;
}

void TextView::setScale(double scale)
{
    text_edit->setScale(scale);
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
