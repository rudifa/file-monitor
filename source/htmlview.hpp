
#ifndef HTML_VIEW_HPP
#define HTML_VIEW_HPP

#include "view.hpp"

class CustomWebView;

namespace Ui { class MainWindow; }

class HtmlView : public View
{
    Q_OBJECT

public:
    HtmlView(Ui::MainWindow const & ui, QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri, bool is_reload = false);

    void setScale(double scale);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void selectAll();
    virtual void copy();
    virtual void find();

private slots:
    void slotSetScroll();

protected:
    CustomWebView * web_view;
};

#endif
