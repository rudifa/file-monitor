
#include "textview.hpp"

#include "ui_mainwindow.h"
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

TextView::TextView(Ui::MainWindow const & ui, QWidget * parent)
    : View(ViewScale(ViewScale::Linear, 1, 100, 12), parent),
      text_edit(new CustomTextEdit(ui, view_scale, parent)),
      indent_xml(false), scroll_to_bottom_on_change(false)
{
}

QWidget * TextView::getWidget()
{
    QWidget * widget = dynamic_cast<QWidget *>(text_edit);
    assert(widget);

    return widget;
}

bool TextView::load(QString const & file_uri, bool is_reload)
{
    QFile file(file_uri);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    unformatted_content = in.readAll();

    formatAndInsertContent(unformatted_content, indent_xml);

    connect(text_edit, SIGNAL(signalScaleChanged(double)), SLOT(slotScaleChanged(double)));

    // Only scroll bottom if we are reloading non-xml content.
    if (scroll_to_bottom_on_change && is_reload && !xml::isXML(unformatted_content))
        QTimer::singleShot(0, this, SLOT(slotScrollToBottom()));

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

void TextView::scrollToBottomOnChange(bool scroll_to_bottom)
{
    scroll_to_bottom_on_change = scroll_to_bottom;
}

void TextView::selectAll()
{
    text_edit->selectAll();
}

void TextView::copy()
{
    text_edit->copy();
}

void TextView::slotFindNext(QString const & text, bool case_sensitive)
{
    QTextDocument::FindFlags find_flags = static_cast<QTextDocument::FindFlags>(0);
    if (case_sensitive)
        find_flags = find_flags | QTextDocument::FindCaseSensitively;

    case_sensitive ? QTextDocument::FindCaseSensitively : static_cast<QTextDocument::FindFlags>(0);
    bool success = text_edit->find(text, find_flags);

    // We either didn't find the text or hit the last instance of the text.
    if (!success)
    {
        // This addresses a bug in which find fails when the text to be found exists below a currently selected section.
        if (text_edit->toPlainText().contains(text, case_sensitive ? Qt::CaseSensitive : Qt::CaseInsensitive))
        {
            // If nothing is selected, then this is a strange error case we have to figure out.
            assert(!text_edit->textCursor().selectedText().isEmpty());

            // Remove selection, find again, and bail if we found another (otherwise, fall through to "Find First" below).
            text_edit->textCursor().clearSelection();
            if (text_edit->find(text, find_flags))
                return;
        }

        // Wrap around by moving to the first instance of the text in the document.
        find_flags = find_flags | QTextDocument::FindBackward;
        while (text_edit->find(text, find_flags))
            ;
    }
}

void TextView::slotFindPrevious(QString const & text, bool case_sensitive)
{
    QTextDocument::FindFlags find_flags = QTextDocument::FindBackward;
    if (case_sensitive)
        find_flags = find_flags | QTextDocument::FindCaseSensitively;

    bool success = text_edit->find(text, find_flags);

    // We either didn't find the text or hit the first instance of the text.
    if (!success)
    {
        // NOTE: If no text is selected, find previous fails.  We must find forward first.
        if (text_edit->textCursor().selectedText().isEmpty())
            text_edit->find(text);

        // Wrap around by moving to the last instance of the text in the document.
        find_flags = find_flags ^ QTextDocument::FindBackward;
        while (text_edit->find(text, find_flags))
            ;
    }
}

void TextView::formatAndInsertContent(QString const & content, bool indent)
{
    QDomDocument document;
    if (indent && document.setContent(content))
        text_edit->setPlainText(document.toString(4));
    else
        text_edit->setPlainText(content);
}

void TextView::slotScrollToBottom()
{
    text_edit->verticalScrollBar()->setSliderPosition(text_edit->verticalScrollBar()->maximumHeight());
}
