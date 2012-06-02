
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "tabwidget.hpp"

#include <QSpacerItem>
#include <QGridLayout>
#include <QSlider>
#include <QDropEvent>
#include <QUrl>

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
    connect(ui->action_transparent_background, SIGNAL(toggled(bool)), tab_widget, SLOT(slotEnableTransparentBackground(bool)));

    // Window location and dimensions.
    QRect default_dimensions(100, 100, 500, 400);
    setGeometry(settings.value("appGeometry", default_dimensions).toRect());

    // User defined settings.
    ui->action_close_deleted_files->setChecked(settings.value("closeDeletedFiles", true).toBool());
    ui->action_transparent_background->setChecked(settings.value("transparentBackground", true).toBool());
}

MainWindow::~MainWindow()
{
    settings.setValue("appGeometry", geometry());
    settings.setValue("closeDeletedFiles", ui->action_close_deleted_files->isChecked());
    settings.setValue("transparentBackground", ui->action_transparent_background->isChecked());
    delete ui;
}

// Add this kind of code to every widget with translatable strings when we want the app
//  to retranslate on the fly. "Internationalization with Qt"
void MainWindow::changeEvent(QEvent * event)
{
    QMainWindow::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent * event)
{
    event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent * event)
{
    event->acceptProposedAction();
}
void MainWindow::dragLeaveEvent(QDragLeaveEvent * event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent * event)
{
    QMimeData const * mime_data = event->mimeData();

    if (mime_data->hasUrls())
    {
        QStringList path_list;
        QList<QUrl> url_list = mime_data->urls();

        int max_num_files = 32;
        for (int i = 0; i < url_list.size() && i < max_num_files; ++i)
            path_list.append(url_list.at(i).toLocalFile());

        tab_widget->openFiles(path_list);
    }
}
