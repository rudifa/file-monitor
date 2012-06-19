
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QSettings>

#include "ui_mainwindow.h"

namespace Ui
{
    class MainWindow;
}

class TabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget * parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent * event);

    void dragEnterEvent(QDragEnterEvent * event);
    void dragMoveEvent(QDragMoveEvent * event);
    void dragLeaveEvent(QDragLeaveEvent * event);
    void dropEvent(QDropEvent * event);

private slots:
    void slotSaveSession();
    void slotRestoreSession();

private:
    Ui::MainWindow ui;
    TabWidget * tab_widget;
    QSettings settings;

    void loadSettings();
    void saveSettings();
};

#endif
