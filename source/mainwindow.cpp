
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "tabwidget.hpp"

#include <QSpacerItem>
#include <QGridLayout>
#include <QSlider>

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget * central_widget = new QWidget(this);
    QGridLayout * grid_layout = new QGridLayout(central_widget);
    grid_layout->setMargin(0);
    grid_layout->addItem(new QSpacerItem(0, 4, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 0);

    tab_widget = new TabWidget(ui, this);
    grid_layout->addWidget(tab_widget, 1, 0);
    setCentralWidget(central_widget);

    setContentsMargins(2, 0, 2, 0);
    connect(ui->action_file_open, SIGNAL(triggered()), tab_widget, SLOT(slotLoadFile()));
    connect(ui->action_file_close, SIGNAL(triggered()), tab_widget, SLOT(slotCloseCurrentTab()));

    // Window location and dimensions.
    setGeometry(settings.value("appGeometry", QRect(100, 100, 500, 400)).toRect());
}

MainWindow::~MainWindow()
{
    settings.setValue("appGeometry", geometry());
    delete ui;
}

// Add this kind of code to every widget with translatable strings when we want the app
//  to retranslate on the fly. "Internationalization with Qt"
void MainWindow::changeEvent(QEvent * e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
