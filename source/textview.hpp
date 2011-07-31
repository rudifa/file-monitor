
#ifndef TEXT_VIEW_HPP
#define TEXT_VIEW_HPP

#include "view.hpp"

class QTextEdit;

class TextView : public View
{
public:
    TextView(QWidget * parent);
    ~TextView();

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setVerticalScroll(int scroll);
    int getVerticalScroll() const;
    void setHorizontalScroll(int scroll);
    int getHorizontalScroll() const;

protected:
    QTextEdit * text_edit;
};

#endif
