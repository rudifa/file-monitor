
#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "view.hpp"

class QLabel;
class QScrollArea;

class ImageView : public View
{
public:
    ImageView(QWidget * parent);
    ~ImageView();

    QWidget * getWidget();
    bool load(QString const & file_uri);

    void setZoom(double zoom);
    void setVerticalScroll(int scroll);
    int getVerticalScroll() const;
    void setHorizontalScroll(int scroll);
    int getHorizontalScroll() const;

protected:
    QLabel * image_label;
    QScrollArea * scroll_area;
};

#endif
