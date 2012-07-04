
#ifndef TEXT_VIEW_HPP
#define TEXT_VIEW_HPP

#include "view.hpp"

class CustomTextEdit;

class TextView : public View
{
public:
    TextView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setScale(double scale);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void wordWrap(bool word_wrap);
    virtual void indentXML(bool indent);

protected:
    CustomTextEdit * text_edit;

    QString unformatted_content;
    bool indent_xml;
    void formatAndInsertContent(QString const & content, bool indent);
};

#endif
