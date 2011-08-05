
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
    ~TabPage();

    bool load(QString const & uri);
    
    QString getUri() const;
    int getZoom() const;

    static double const zoom_min;
    static double const zoom_max;
    static double const zoom_step;

signals:
    void signalUserChangedZoom(int zoom);
    
public slots:
    void slotZoomIn();
    void slotZoomOut();
    void slotSetZoom(int zoom);
    void slotLoadSettings();

private slots:
    void slotReload();
    void slotSaveSettings();

private:
    View * view;
    View * createView(QString const & file_uri);

    QString file_uri;
    QSettings settings;

    bool zoomIsValid(double zoom) const;
};

#endif
