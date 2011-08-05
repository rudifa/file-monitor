
#ifndef TAB_WIDGET_HPP
#define TAB_WIDGET_HPP

#include <QTabWidget>
#include <QSettings>

#include <set>

namespace Ui
{
    class MainWindow;
}

class QSlider;
class QMouseMoveEvent;
class TabPage;
class FileDisplay;
class QFileSystemWatcher;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:    
    explicit TabWidget(Ui::MainWindow * ui, QWidget * parent = 0);
    ~TabWidget();

    void updateTabConnections();
    void openFiles(QStringList file_uris);

    void mouseMoveEvent(QMouseEvent * event);

public slots:
    void slotLoadFile();
    void slotCloseCurrentTab();

private slots:
    void slotRemoveTab(int tab_index);
    void slotCurrentTabChanged();
    void slotFileChanged(QString changed_file_uri);
    void slotSynchronizeZoomSlider();

private:
    Ui::MainWindow * ui;
    QSlider * zoom_slider;
    TabPage * uriTabPage(QString const & uri) const;
    QString tabUri(QWidget * tab_widget) const;
    QStringList allTabUris() const;
    QFileSystemWatcher * file_watcher;
    QSettings settings;

    TabPage * loadFile(QString const & file_uri);
};

#endif
