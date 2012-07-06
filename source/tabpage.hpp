
#ifndef TAB_PAGE_HPP
#define TAB_PAGE_HPP

#include <QWidget>
#include <QSettings>

class View;
class QShowEvent;

class TabPage : public QWidget
{
    Q_OBJECT

public:
    enum FileType { TEXT, HTML, SVG, IMAGE };

    TabPage(QWidget * parent = 0);

    bool load(QString const & uri);

    void enableTransparentBackground(bool enable);
    void wordWrap(bool word_wrap);
    void indentXML(bool indent_xml);
    void scrollToBottomOnChange(bool scroll_to_bottom);
    
    QString getUri() const;
    int getPercentageZoom() const;

    inline bool wasCurrentTab() const { return displayed_to_user; }
    inline void setCurrentTab() { displayed_to_user = true; }

signals:
    void signalScaleChanged();
    
public slots:
    void slotZoomIn();
    void slotZoomOut();
    void slotResetZoom();
    void slotSetZoom(int zoom);
    void slotSaveSettings();
    void slotLoadSettings();

private slots:
    void slotReload();

private:
    View * view;
    View * createView(QString const & file_uri);

    QString file_uri;
    QSettings settings;

    // true if this TabPage was the currentTab at least once.
    bool displayed_to_user;

    bool zoomIsValid(double zoom) const;
};

#endif
