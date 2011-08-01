
#ifndef TAB_PAGE_HPP
#define TAB_PAGE_HPP

#include <QWidget>
#include <QSettings>

class View;
class QFileSystemWatcher;

class TabPage : public QWidget
{
    Q_OBJECT

public:
    enum FileType { TEXT, HTML, SVG, IMAGE };

    TabPage(QWidget * parent = 0);
    ~TabPage();

    bool load(QString const & uri);
    
    QString getUri() const;
    QFileSystemWatcher * getFileWatcher();
    int getZoom() const;
    void loadSettings();

    static double const zoom_min;
    static double const zoom_max;
    static double const zoom_step;

signals:
    void signalZoomChanged(int zoom);
    
public slots:
    void slotZoomIn();
    void slotZoomOut();
    void slotSetZoom(int zoom);

private slots:
    void slotReload();

private:
    View * view;
    QString file_uri;
    QFileSystemWatcher * file_watcher;
    QSettings settings;

    void saveSettings();
    View * createView(QString const & file_uri);

    bool zoomIsValid(double zoom) const;
};

#endif
