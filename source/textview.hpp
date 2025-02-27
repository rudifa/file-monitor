
#ifndef TEXT_VIEW_HPP
#define TEXT_VIEW_HPP

#include "view.hpp"

class CustomTextEdit;

namespace Ui
{
class MainWindow;
}

class TextView : public View
{
    Q_OBJECT

   public:
    TextView(Ui::MainWindow const& ui, QWidget* parent);

    QWidget* getWidget();
    bool load(QString const& file_uri, bool is_reload = false);

    void setScale(double scale);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

    virtual void wordWrap(bool word_wrap);
    virtual void indentXML(bool indent);
    virtual void scrollToBottomOnChange(bool scroll_to_bottom);

    virtual void selectAll();
    virtual void copy();

   public slots:
    virtual void slotFindNext(QString const& text, bool case_sensitive);
    virtual void slotFindPrevious(QString const& text, bool case_sensitive);

   protected:
    CustomTextEdit* text_edit;

    QString unformatted_content;
    bool indent_xml;
    bool scroll_to_bottom_on_change;

    void formatAndInsertContent(QString const& content, bool indent);

   private slots:
    void slotScrollToBottom();
};

#endif
