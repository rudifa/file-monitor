
#ifndef TAB_PAGE_HPP
#define TAB_PAGE_HPP

#include <QWidget>
#include <QSettings>

class View;
class QShowEvent;

namespace Ui { class MainWindow; }

class TabWidget; // Forward declaration

class TabPage : public QWidget
{
    Q_OBJECT

public:
    enum FileType { TEXT, HTML, SVG, IMAGE };
    TabPage(Ui::MainWindow const &ui, TabWidget *tabWidget, QWidget *parent = 0);

    bool load(QString const & uri);

    void makeBackgroundTransparent(bool transparent);
    void wordWrap(bool word_wrap);
    void indentXML(bool indent_xml);
    void scrollToBottomOnChange(bool scroll_to_bottom);

    QString getUri() const;
    bool isText() const;

    int getPercentageZoom() const;

    inline bool wasCurrentTab() const { return displayed_to_user; }
    inline void setCurrentTab() { displayed_to_user = true; }

    void applyZoomToAllTabs(int zoom);

signals:
    void signalScaleChanged();
    void signalFindNext(QString const & text, bool case_sensitive);
    void signalFindPrevious(QString const & text, bool case_sensitive);

public slots:
    void slotZoomLock();
    void slotZoomIn();
    void slotZoomOut();
    void slotResetZoom();
    void slotSelectAll();
    void slotCopy();

    void slotSetZoom(int zoom);
    void slotSaveSettings();
    void slotLoadSettings();

private slots:
    void slotReload();

private:
    Ui::MainWindow const & ui;
    View * view;
    View * createView(QString const & file_uri);

    QString file_uri;
    FileType file_type;
    QSettings settings;

    // true if this TabPage was the currentTab at least once.
    bool displayed_to_user;

    int m_lockedZoom;

    TabWidget *m_tabWidget;
};

#endif
