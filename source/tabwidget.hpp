
#ifndef TAB_WIDGET_HPP
#define TAB_WIDGET_HPP

#include <QSettings>
#include <QStringList>
#include <QTabWidget>
#include <memory>

namespace Ui
{
class MainWindow;
}

class QSlider;
class QMouseMoveEvent;
class QWheelEvent;

class FileDisplay;
class FileSystemWatcher;
class TabPage;
class FindDialog;

class TabWidget : public QTabWidget
{
    Q_OBJECT

   public:
    explicit TabWidget(Ui::MainWindow const &ui, QWidget *parent = 0);

    void updateTabConnections();
    void openFiles(QStringList file_uris);
    void saveSettings();
    void loadSettings();

    void mouseMoveEvent(QMouseEvent *event);

   public slots:
    void slotOpenFile(QString const &file_uri);
    void slotOpenFiles();
    void slotOpenFiles(QStringList const &file_uris);
    void slotCloseCurrentTab();
    void slotCloseAllButCurrentTabPage();
    void slotCloseAllTabPages();

    void slotMakeBackgroundTransparent(bool transparent);
    void slotWordWrap(bool word_wrap);
    void slotIndentXML(bool indent_xml);
    void slotScrollToBottomOnChange(bool scroll_to_bottom);

   private slots:
    void slotOpenRecentFile();
    void slotRemoveTab(int tab_index);
    void slotCurrentTabChanged(int new_index);
    void slotFileChanged(QString changed_file_uri);
    void slotSynchronizeZoomSlider();
    void slotFind();

   private:
    Ui::MainWindow const &ui;
    QSlider *zoom_slider;
    FileSystemWatcher *file_watcher;
    FindDialog *find_dialog;
    QSettings settings;

    TabPage *loadFile(QString const &file_uri);

    TabPage *tabPage(QWidget *widget) const;
    std::vector<TabPage *> allTabPages() const;
    TabPage *uriTabPage(QString const &uri) const;
    QString tabUri(QWidget *tab_widget) const;
    QString currentTabUri() const;
    QStringList allTabUris() const;

    QStringList allRecentlyClosedFilesOnDisk() const;
    void updateRecentFiles();

    void updateActionEnables();
};

#endif
