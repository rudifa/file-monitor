
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
class TabPage;
class FileDisplay;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:    
    explicit TabWidget(Ui::MainWindow * ui, QWidget * parent = 0);
    ~TabWidget();

    void updateTabConnections();
    void openFiles(QStringList file_uris);

public slots:
    void slotLoadFile();
    void slotCloseCurrentTab();

private slots:
    void slotRemoveTab(int index);
    void slotCurrentTabChanged();
    void slotFileChanged(QString changed_file_uri);

private:
    Ui::MainWindow * ui;
    QSlider * zoom_slider;
    TabPage * uriTabPage(QString const & uri) const;
    QString tabUri(QWidget * tab_widget) const;
    QStringList allTabUris() const;
    QSettings settings;

    TabPage * loadFile(QString const & file_uri);
    void synchronizeZoomSlider();

    std::set<QString> loaded_uris;
};

#endif
