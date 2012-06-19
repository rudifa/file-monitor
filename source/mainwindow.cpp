
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

    QWidget * central_widget = new QWidget(this);
    QGridLayout * grid_layout = new QGridLayout(central_widget);
    grid_layout->setMargin(0);
    grid_layout->addItem(new QSpacerItem(0, 4, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 0);

    tab_widget = new TabWidget(ui, this);
    grid_layout->addWidget(tab_widget, 1, 0);
    setCentralWidget(central_widget);

    setContentsMargins(2, 0, 2, 0);
    connect(ui.action_file_open, SIGNAL(triggered()), tab_widget, SLOT(slotLoadFile()));
    connect(ui.action_file_close, SIGNAL(triggered()), tab_widget, SLOT(slotCloseCurrentTab()));
    connect(ui.action_transparent_background, SIGNAL(toggled(bool)), tab_widget, SLOT(slotEnableTransparentBackground(bool)));
    connect(ui.action_word_wrap, SIGNAL(toggled(bool)), tab_widget, SLOT(slotWordWrap(bool)));
    connect(ui.action_indent_xml, SIGNAL(toggled(bool)), tab_widget, SLOT(slotIndentXML(bool)));
    connect(ui.action_save_session, SIGNAL(triggered()), SLOT(slotSaveSession()));
    connect(ui.action_restore_session, SIGNAL(triggered()), SLOT(slotRestoreSession()));
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
    tab_widget->loadSettings();
}

// TODO: look into refreshing this text when the application language changes.
void MainWindow::slotAboutFileMonitor()
{
    auto content = QString("<img src=\":/icons/file-monitor.xpm\" style=\"float: left; \"/>"
        "<p style=\"margin-left: 40px; margin-top: -20px;\">%1</p><p>%2</p><p>%3</p><p>%4</p><p>%5</p>")
    .arg(tr("<b>File Monitor</b> is a cross-platform file viewer that automatically redraws files when they change on disk."))
    .arg(tr("You can edit an HTML document in a simple text editor and watch it update in real-time using File Monitor."))
    .arg(tr("If you are keeping track of 10 different log files, File Monitor will automatically switch to the most recently "
           "modified file and take you to those changes."))
    .arg(tr("Currently supported file types are text, html, svg, and raster image file formats (jpg, gif, png, bmp, etc)."))
    .arg(tr("To learn more about File Monitor or to contribute, please visit our project page "
        "<a href=\"http://code.google.com/p/file-monitor\">http://code.google.com/p/file-monitor</a>"));

    auto about_dialog = new QDialog(QApplication::activeModalWidget());
    about_dialog->setModal(true);
    about_dialog->setWindowTitle(tr("About File Monitor"));
    about_dialog->setWindowIcon(QIcon(":/icons/about.png"));

    QGridLayout * layout = new QGridLayout(about_dialog);
    layout->setSpacing(14);
    about_dialog->setLayout(layout);

    QLabel * about_text_label = new QLabel(content, about_dialog);
    about_text_label->setWordWrap(true);
    about_text_label->setTextFormat(Qt::RichText);
    about_text_label->setOpenExternalLinks(true);
    layout->addWidget(about_text_label, 0, 1, 1, 1, Qt::AlignTop);

    QPushButton * about_close_button = new QPushButton(tr("&Close"), about_dialog);
    layout->addWidget(about_close_button, 1, 1, 1, 2, Qt::AlignRight);
    connect(about_close_button, SIGNAL(clicked()), about_dialog, SLOT(close()));

    layout->setSizeConstraint(QLayout::SetFixedSize);
    about_dialog->show();
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
}

void MainWindow::saveSettings()
{
    settings.setValue("app_geometry", geometry());
    settings.setValue("close_deleted_files", ui.action_close_deleted_files->isChecked());
    settings.setValue("transparent_background", ui.action_transparent_background->isChecked());
    settings.setValue("word_wrap", ui.action_word_wrap->isChecked());
    settings.setValue("indent_xml", ui.action_indent_xml->isChecked());

    tab_widget->saveSettings();
}
