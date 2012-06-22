
#ifndef TAB_WIDGET_HPP
#define TAB_WIDGET_HPP

#include <QTabWidget>
#include <QSettings>

#include <memory>

namespace Ui { class MainWindow; }

class QSlider;
class QMouseMoveEvent;
class QWheelEvent;

class FileDisplay;
class FileSystemWatcher;
class TabPage;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:    
    explicit TabWidget(std::unique_ptr<Ui::MainWindow> const & ui, QWidget * parent = 0);

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
    void slotCurrentTabChanged(int new_index);
    void slotFileChanged(QString changed_file_uri);
    void slotSynchronizeZoomSlider();

private:
    std::unique_ptr<Ui::MainWindow> const & ui;
    QSlider * zoom_slider;
    FileSystemWatcher * file_watcher;
    QSettings settings;

    TabPage * loadFile(QString const & file_uri);

    TabPage * tabPage(QWidget * widget) const;
    std::vector<TabPage *> allTabPages() const;
    TabPage * uriTabPage(QString const & uri) const;
    QString tabUri(QWidget * tab_widget) const;
    QStringList allTabUris() const;
};

#endif
