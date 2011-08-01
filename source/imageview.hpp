
#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "view.hpp"

#include <QObject>

class QLabel;
class QScrollArea;

class ImageView : public QObject, public View
{
    Q_OBJECT

public:
    ImageView(QWidget * parent);

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setScrollDimensions(QPoint dimensions);
    QPoint getScrollDimensions() const;

private slots:
    void slotSetScroll();

protected:
    QLabel * image_label;
    QScrollArea * scroll_area;
};

#endif
