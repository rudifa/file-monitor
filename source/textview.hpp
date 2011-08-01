
#ifndef TEXT_VIEW_HPP
#define TEXT_VIEW_HPP

#include "view.hpp"

#include <QObject>

class QTextEdit;

class TextView : public QObject, public View
{
    Q_OBJECT

public:
    TextView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

private slots:
    void slotSetScroll();

protected:
    QTextEdit * text_edit;
};

#endif
