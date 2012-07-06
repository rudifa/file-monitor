
#ifndef TEXT_VIEW_HPP
#define TEXT_VIEW_HPP

#include "view.hpp"

class CustomTextEdit;

class TextView : public View
{
    Q_OBJECT

public:
    TextView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri, bool is_reload = false);

    void setScale(double scale);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void wordWrap(bool word_wrap);
    virtual void indentXML(bool indent);
    virtual void scrollToBottomOnChange(bool scroll_to_bottom);

protected:
    CustomTextEdit * text_edit;

    QString unformatted_content;
    bool indent_xml;
    bool scroll_to_bottom_on_change;

    void formatAndInsertContent(QString const & content, bool indent);

private slots:
    void slotScrollToBottom();
};

#endif
