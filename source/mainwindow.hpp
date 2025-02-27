
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QSettings>

#include "ui_mainwindow.h"

class TabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    bool isZoomLocked() const { return ui.action_lock_zoom->isChecked(); }

   protected:
    // TODO: Internationalization needs work!
    void changeEvent(QEvent* event);

    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);

   private slots:
    // TODO: Consider abstracting away the session files with a "Session
    // Manager".
    void slotSaveSessionAs();
    void slotOpenSession();
    void slotAboutFileMonitor();

   private:
    Ui::MainWindow ui;
    TabWidget* tab_widget;
    QSettings settings;

    void loadSettings();
    void saveSettings();
};

#endif
