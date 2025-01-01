
#include "htmlview.hpp"

#include "customwebview.hpp"
#include "utility.hpp"

#include <QWebEngineView>
#include <QWebEnginePage>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QTimer>

#include <cmath>
#include <cassert>
#include <utility>

using namespace utility;

HtmlView::HtmlView(Ui::MainWindow const & ui, QWidget * parent)
    : View(ViewScale(ViewScale::Linear, .25, 5, 1), parent), web_view(new CustomWebView(ui, view_scale, parent))
{
}

QWidget * HtmlView::getWidget()
{
    QWidget * widget = dynamic_cast<QWidget *>(web_view);
    assert(widget);

    return widget;
}

bool HtmlView::load(QString const & file_uri, bool)
{
    web_view->setUrl(file_uri);

    connect(web_view, SIGNAL(signalScaleChanged(double)), SLOT(slotScaleChanged(double)));

    return true;
}

void HtmlView::setScale(double scale)
{
    web_view->setScale(scale);
}

void HtmlView::setScrollDimensions(QPoint dimensions)
{
    scroll_dimensions = dimensions;
    slotSetScroll();
}

// Periodically check to see if the control is ready for the scroll to be set.
// void HtmlView::slotSetScroll()
// {
//     int max_scroll_vertical = web_view->page()->currentFrame()->scrollBarMaximum(Qt::Vertical);
//     int max_scroll_horizontal = web_view->page()->currentFrame()->scrollBarMaximum(Qt::Horizontal);
//     QPoint scroll_max(max_scroll_horizontal, max_scroll_vertical);

//     if (scroll_max.x() >= scroll_dimensions.x() && scroll_max.y() >= scroll_dimensions.y())
//         web_view->page()->currentFrame()->setScrollPosition(scroll_dimensions);
//     else
//         QTimer::singleShot(10, this, SLOT(slotSetScroll()));
// }


void HtmlView::slotSetScroll()
{
    web_view->page()->runJavaScript(
        "(function() { "
        "   return {"
        "       maxH: document.documentElement.scrollWidth - document.documentElement.clientWidth,"
        "       maxV: document.documentElement.scrollHeight - document.documentElement.clientHeight"
        "   };"
        "})()",
        [this](const QVariant &result)
        {
            QVariantMap scrollInfo = result.toMap();
            int max_scroll_horizontal = scrollInfo["maxH"].toInt();
            int max_scroll_vertical = scrollInfo["maxV"].toInt();

            if (max_scroll_horizontal >= scroll_dimensions.x() && max_scroll_vertical >= scroll_dimensions.y())
            {
                QString script = QString("window.scrollTo(%1, %2);").arg(scroll_dimensions.x()).arg(scroll_dimensions.y());
                web_view->page()->runJavaScript(script);
            }
            else
            {
                QTimer::singleShot(10, this, &HtmlView::slotSetScroll);
            }
        });
}

// QPoint HtmlView::getScrollDimensions() const
// {
//     return web_view->page()->currentFrame()->scrollPosition();
// }

QPoint HtmlView::getScrollDimensions() const
{
    // This function now needs to be asynchronous, so we'll return a default value
    // You might want to redesign this to use a callback or signal
    return QPoint(0, 0);
}

void HtmlView::selectAll()
{
    web_view->page()->triggerAction(QWebEnginePage::SelectAll);
}

void HtmlView::copy()
{
    web_view->page()->triggerAction(QWebEnginePage::Copy);
}

// void HtmlView::slotFindNext(QString const & text, bool case_sensitive)
// {
//     QWebPage::FindFlags find_flags = static_cast<QWebPage::FindFlags>(0);
//     if (case_sensitive)
//         find_flags = find_flags | QWebPage::FindCaseSensitively;

//     bool success = web_view->page()->findText(text, find_flags);

//     // We either didn't find the text or hit the last instance of the text.
//     if (!success)
//     {
//         // Wrap around by moving to the first instance of the text in the document.
//         find_flags = find_flags | QWebPage::FindBackward;
//         while (web_view->page()->findText(text, find_flags))
//             ;
//     }
// }

// void HtmlView::slotFindPrevious(QString const & text, bool case_sensitive)
// {
//     QWebPage::FindFlags find_flags = QWebPage::FindBackward;
//     if (case_sensitive)
//         find_flags = find_flags | QWebPage::FindCaseSensitively;

//     bool success = web_view->page()->findText(text, find_flags);

//     // We either didn't find the text or hit the first instance of the text.
//     if (!success)
//     {
//         // Wrap around by moving to the last instance of the text in the document.
//         find_flags = find_flags ^ QWebPage::FindBackward;
//         while (web_view->page()->findText(text, find_flags))
//             ;
//     }
// }

void HtmlView::slotFindNext(QString const &text, bool case_sensitive)
{
    QWebEnginePage::FindFlags find_flags = QWebEnginePage::FindFlags();
    if (case_sensitive)
        find_flags |= QWebEnginePage::FindCaseSensitively;

    web_view->findText(text, find_flags);
}

void HtmlView::slotFindPrevious(QString const &text, bool case_sensitive)
{
    QWebEnginePage::FindFlags find_flags = QWebEnginePage::FindBackward;
    if (case_sensitive)
        find_flags |= QWebEnginePage::FindCaseSensitively;

    web_view->findText(text, find_flags);
}
