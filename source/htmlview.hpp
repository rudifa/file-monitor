
#ifndef HTML_VIEW_HPP
#define HTML_VIEW_HPP

#include "view.hpp"

class CustomWebView;

class HtmlView : public View
{
    Q_OBJECT

public:
    HtmlView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setScale(double scale);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

private slots:
    void slotSetScroll();

protected:
    CustomWebView * web_view;
};

#endif
