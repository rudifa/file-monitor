
#include "mainwindow.hpp"

#include "tabwidget.hpp"
#include "utility.hpp"

#include <QSpacerItem>
#include <QGridLayout>
#include <QSlider>
#include <QDropEvent>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>

using namespace utility;

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    tab_widget = new TabWidget(ui, this);
    setCentralWidget(tab_widget);

    connect(ui.action_quit, SIGNAL(triggered()), SLOT(close()));
    connect(ui.action_session_save_as, SIGNAL(triggered()), SLOT(slotSaveSessionAs()));
    connect(ui.action_session_open, SIGNAL(triggered()), SLOT(slotOpenSession()));
    connect(ui.action_about_file_monitor, SIGNAL(triggered()), SLOT(slotAboutFileMonitor()));

    settings.beginGroup("global");
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
}

// Add this kind of code to every widget with translatable strings when we want the app
//  to retranslate on the fly. "Internationalization with Qt"
void MainWindow::changeEvent(QEvent * event)
{
    QMainWindow::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
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

void MainWindow::slotSaveSessionAs()
{
    QString last_saved_session_uri("");

    QString file_uri = QFileDialog::getSaveFileName(this, tr("Save Session"), last_saved_session_uri, tr("Session (*.ses)"));

    if (file_uri.isEmpty())
        return;

    saveSettings();
    settings::saveSettingsToFile(file_uri);
}

void MainWindow::slotOpenSession()
{
    QString last_restored_session_uri("");

    QString file_uri = QFileDialog::getOpenFileName(this, tr("Restore Session"),
        last_restored_session_uri, tr("Session (*.ses)"));

    if (file_uri.isEmpty())
        return;

    // When each tab page closes it overwrites the current QSettings.
    tab_widget->slotCloseAllTabPages();

    settings::loadSettingsFromFile(file_uri);
    loadSettings();
}

void MainWindow::slotAboutFileMonitor()
{
    QString file_monitor_project_url("http://code.google.com/p/file-monitor");
    QString file_monitor_version("2.0.0");

    auto about_content = QString("<h3>%1</h3><p>%2</p><p>%3</p><p>%4</p><p>%5</p><p>%6%7</p><hr /><p>%8</p><p>%9</p>")
            .arg(tr("File Monitor - ") + file_monitor_version)
    .arg(tr("File Monitor is a cross-platform file viewer that automatically redraws files when they change on disk."))
    .arg(tr("You can edit an HTML document in a simple text editor and watch it update in real-time using File Monitor."))
    .arg(tr("If you are keeping track of 10 different log files, File Monitor will automatically switch to the most recently "
           "modified file and take you to those changes."))
    .arg(tr("Currently supported file types are text, html, svg, and raster image file formats (jpg, gif, png, bmp, etc)."))
    .arg(tr("To learn more about File Monitor or to contribute, please visit our project page "))
    .arg("<a href=\"" + file_monitor_project_url + "\">" + file_monitor_project_url + "</a>")
    .arg(tr("Copyright 2010-2012 Mark Turney.  All rights reserved."))
    .arg(tr("This program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE."));

    QMessageBox::about(this, tr("About File Monitor"), about_content);
}

void MainWindow::loadSettings()
{
    // Window location and dimensions.
    QRect default_dimensions(100, 100, 500, 400);
    setGeometry(settings.value("app_geometry", default_dimensions).toRect());

    // User defined settings.
    ui.action_close_deleted_files->setChecked(settings.value("closeDeletedFiles", false).toBool());
    ui.action_transparent_background->setChecked(settings.value("transparent_background", true).toBool());
    ui.action_word_wrap->setChecked(settings.value("word_wrap", true).toBool());
    ui.action_indent_xml->setChecked(settings.value("indentXML", true).toBool());
    ui.action_scroll_to_bottom->setChecked(settings.value("scrollToBottom", false).toBool());

    tab_widget->loadSettings();
}

void MainWindow::saveSettings()
{
    settings.setValue("app_geometry", geometry());
    settings.setValue("close_deleted_files", ui.action_close_deleted_files->isChecked());
    settings.setValue("transparent_background", ui.action_transparent_background->isChecked());
    settings.setValue("word_wrap", ui.action_word_wrap->isChecked());
    settings.setValue("indent_xml", ui.action_indent_xml->isChecked());
    settings.setValue("scrollToBottom", ui.action_scroll_to_bottom->isChecked());

    tab_widget->saveSettings();
}
