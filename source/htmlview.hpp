
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

public slots:
    virtual void slotFindNext(QString const & text, bool case_sensitive);
    virtual void slotFindPrevious(QString const & text, bool case_sensitive);

private slots:
    void slotSetScroll();

protected:
    CustomWebView * web_view;

    // Holds the desired scroll dimensions while the HTML control renders.
    QPoint scroll_dimensions;
};

#endif
