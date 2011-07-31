
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
    ~HtmlView();

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setVerticalScroll(int scroll);
    int getVerticalScroll() const;
    void setHorizontalScroll(int scroll);
    int getHorizontalScroll() const;

public slots:
    void slotSetScroll();

private slots:
    void slotLoadDeferredScroll();

protected:
    QWebView * web_view;

private:
    int deferred_vertical_scroll;
    int deferred_horizontal_scroll;
};

#endif
