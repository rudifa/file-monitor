
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QSettings>

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
    void changeEvent(QEvent * e);

private:
    Ui::MainWindow * ui;
    TabWidget * tab_widget;
    QSettings settings;
};

#endif
