
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
