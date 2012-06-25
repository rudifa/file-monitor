
#include "mainwindow.hpp"

#include "ui_mainwindow.h"
#include "tabwidget.hpp"
#include "utility.hpp"
#include "aboutdialog.hpp"

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
    QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    tab_widget = new TabWidget(ui, this);
    setCentralWidget(tab_widget);

    connect(ui->action_file_open, SIGNAL(triggered()), tab_widget, SLOT(slotLoadFile()));
    connect(ui->action_file_close, SIGNAL(triggered()), tab_widget, SLOT(slotCloseCurrentTab()));
    connect(ui->action_transparent_background, SIGNAL(toggled(bool)), tab_widget, SLOT(slotEnableTransparentBackground(bool)));
    connect(ui->action_word_wrap, SIGNAL(toggled(bool)), tab_widget, SLOT(slotWordWrap(bool)));
    connect(ui->action_indent_xml, SIGNAL(toggled(bool)), tab_widget, SLOT(slotIndentXML(bool)));
    connect(ui->action_save_session, SIGNAL(triggered()), SLOT(slotSaveSession()));
    connect(ui->action_restore_session, SIGNAL(triggered()), SLOT(slotRestoreSession()));
    connect(ui->action_about_file_monitor, SIGNAL(triggered()), SLOT(slotAboutFileMonitor()));

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

void MainWindow::slotSaveSession()
{
    QString last_saved_session_uri("");

    QString file_uri = QFileDialog::getSaveFileName(this, tr("Save Session"), last_saved_session_uri, tr("Session (*.ses)"));

    if (file_uri.isEmpty())
        return;

    saveSettings();
    settings::saveSettingsToFile(file_uri);
}

void MainWindow::slotRestoreSession()
{
    QString last_restored_session_uri("");

    QString file_uri = QFileDialog::getOpenFileName(this, tr("Restore Session"),
        last_restored_session_uri, tr("Session (*.ses)"));

    if (file_uri.isEmpty())
        return;

    // When each tab page closes it overwrites the current QSettings.
    tab_widget->closeAllTabPages();

    settings::loadSettingsFromFile(file_uri);
    loadSettings();
}

void MainWindow::slotAboutFileMonitor()
{
    AboutDialog about_dialog(QApplication::activeModalWidget());
    about_dialog.exec();
}

void MainWindow::loadSettings()
{
    // Window location and dimensions.
    QRect default_dimensions(100, 100, 500, 400);
    setGeometry(settings.value("app_geometry", default_dimensions).toRect());

    // User defined settings.
    ui->action_close_deleted_files->setChecked(settings.value("closeDeletedFiles", false).toBool());
    ui->action_transparent_background->setChecked(settings.value("transparent_background", true).toBool());
    ui->action_word_wrap->setChecked(settings.value("word_wrap", true).toBool());
    ui->action_indent_xml->setChecked(settings.value("indentXML", true).toBool());

    tab_widget->loadSettings();
}

void MainWindow::saveSettings()
{
    settings.setValue("app_geometry", geometry());
    settings.setValue("close_deleted_files", ui->action_close_deleted_files->isChecked());
    settings.setValue("transparent_background", ui->action_transparent_background->isChecked());
    settings.setValue("word_wrap", ui->action_word_wrap->isChecked());
    settings.setValue("indent_xml", ui->action_indent_xml->isChecked());

    tab_widget->saveSettings();
}
