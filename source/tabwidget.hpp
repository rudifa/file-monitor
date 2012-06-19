
#ifndef TAB_WIDGET_HPP
#define TAB_WIDGET_HPP

#include <QTabWidget>
#include <QSettings>

#include <cassert>

#include "tabpage.hpp"

namespace Ui { class MainWindow; }

class QSlider;
class QMouseMoveEvent;
class QWheelEvent;
class FileDisplay;
class FileSystemWatcher;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:    
    explicit TabWidget(Ui::MainWindow const & ui, QWidget * parent = 0);
    ~TabWidget();

    void updateTabConnections();
    void openFiles(QStringList file_uris);
    void saveSettings();
    void loadSettings();

    void closeAllTabPages();

    void mouseMoveEvent(QMouseEvent * event);

public slots:
    void slotLoadFile();
    void slotCloseCurrentTab();

    void slotEnableTransparentBackground(bool enable);
    void slotWordWrap(bool word_wrap);
    void slotIndentXML(bool indent_xml);

private slots:
    void slotRemoveTab(int tab_index);
    void slotCurrentTabChanged();
    void slotFileChanged(QString changed_file_uri);
    void slotSynchronizeZoomSlider();

private:
    Ui::MainWindow const & ui;
    QSlider * zoom_slider;
    FileSystemWatcher * file_watcher;
    QSettings settings;


    TabPage * loadFile(QString const & file_uri);

    inline TabPage * tabPage(QWidget * widget) const
    {
        // TabWidget should only have TabPage child Widgets.
        assert(dynamic_cast<TabPage *>(widget));
        return static_cast<TabPage *>(widget);
    }

    std::vector<TabPage *> allTabPages() const;
    TabPage * uriTabPage(QString const & uri) const;

    inline QString tabUri(QWidget * tab_widget) const { return tabPage(tab_widget)->getUri(); }
    QStringList allTabUris() const;

};

#endif
