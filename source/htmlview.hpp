
#ifndef HTML_VIEW_HPP
#define HTML_VIEW_HPP

#include "view.hpp"

#include <QObject>

class QWebView;

class HtmlView : public QObject, public View
{
    Q_OBJECT

public:
    HtmlView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

private slots:
    void slotSetScroll();

protected:
    QWebView * web_view;
};

#endif
