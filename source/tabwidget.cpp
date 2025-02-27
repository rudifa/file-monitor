
#include "tabwidget.hpp"

#include "ui_mainwindow.h"
#include "filesystemwatcher.hpp"
#include "tabpage.hpp"
#include "zoom.hpp"
#include "finddialog.hpp"
#include "utility.hpp"

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QStringList>
#include <QTimer>
#include <QSlider>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRegularExpression>

#include <algorithm>
#include <cassert>

using namespace utility;
using namespace qt_extensions;

TabWidget::TabWidget(Ui::MainWindow const &ui, QWidget *parent)
    : QTabWidget(parent), ui(ui), zoom_slider(new QSlider(Qt::Horizontal, parent)),
      file_watcher(new FileSystemWatcher(this)), find_dialog(new FindDialog(this))
{
    // This is an undocumented interface that I'm using to get around an inode updating issue on ubuntu.
    // Qt5 will have a solution to this issue.
    file_watcher->setObjectName(QLatin1String("_qt_autotest_force_engine_poller"));

    setDocumentMode(true);
    setMovable(true);
    setTabsClosable(true);

    setMouseTracking(true);
    tabBar()->setMouseTracking(true);

    connect(ui.action_file_open, SIGNAL(triggered()), SLOT(slotOpenFiles()));
    connect(ui.action_file_close, SIGNAL(triggered()), SLOT(slotCloseCurrentTab()));
    connect(ui.action_file_close_others, SIGNAL(triggered()), SLOT(slotCloseAllButCurrentTabPage()));
    connect(ui.action_file_close_all, SIGNAL(triggered()), SLOT(slotCloseAllTabPages()));
    connect(ui.action_transparent_background, SIGNAL(toggled(bool)), SLOT(slotMakeBackgroundTransparent(bool)));
    connect(ui.action_word_wrap, SIGNAL(toggled(bool)), SLOT(slotWordWrap(bool)));
    connect(ui.action_indent_xml, SIGNAL(toggled(bool)), SLOT(slotIndentXML(bool)));
    connect(ui.action_scroll_to_bottom, SIGNAL(toggled(bool)), SLOT(slotScrollToBottomOnChange(bool)));
    connect(ui.action_edit_find, SIGNAL(triggered()), SLOT(slotFind()));

    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(slotRemoveTab(int)));
    connect(this, SIGNAL(currentChanged(int)), SLOT(slotCurrentTabChanged(int)));

    connect(file_watcher, SIGNAL(fileChanged(QString)), SLOT(slotFileChanged(QString)));

    QString tool_tip = tr("Drag slider to zoom into or out of the current file.");
    zoom_slider->setToolTip(tool_tip);
    zoom_slider->setStatusTip(tool_tip);
    zoom_slider->setMaximumWidth(400);
    zoom_slider->setRange(zoom::min, zoom::max);
    zoom_slider->setSingleStep(zoom::step);
    ui.tool_bar->insertWidget(ui.action_edit_zoom_in, zoom_slider);

    settings.beginGroup("tabs");

    // Update to default state before we load any files (in case we don't load any files).
    updateActionEnables();
    updateRecentFiles();
}

void TabWidget::updateTabConnections()
{
    // Update tab connections.
    for (int i = 0; i < count(); ++i)
    {
        TabPage *tab_page = tabPage(widget(i));
        if (i == currentIndex())
        {
            connect(ui.action_lock_zoom, SIGNAL(triggered()), tab_page, SLOT(slotZoomLock()));
            connect(ui.action_edit_zoom_in, SIGNAL(triggered()), tab_page, SLOT(slotZoomIn()));
            connect(ui.action_edit_zoom_out, SIGNAL(triggered()), tab_page, SLOT(slotZoomOut()));
            connect(ui.action_edit_zoom_reset, SIGNAL(triggered()), tab_page, SLOT(slotResetZoom()));
            connect(ui.action_edit_select_all, SIGNAL(triggered()), tab_page, SLOT(slotSelectAll()));
            connect(ui.action_edit_copy, SIGNAL(triggered()), tab_page, SLOT(slotCopy()));
            connect(ui.action_file_reload, SIGNAL(triggered()), tab_page, SLOT(slotReload()));
            connect(zoom_slider, SIGNAL(valueChanged(int)), tab_page, SLOT(slotSetZoom(int)));
            connect(tab_page, SIGNAL(signalScaleChanged()), this, SLOT(slotSynchronizeZoomSlider()));
            connect(find_dialog, SIGNAL(signalFindNext(QString const &, bool)), tab_page, SIGNAL(signalFindNext(QString const &, bool)));
            connect(find_dialog, SIGNAL(signalFindPrevious(QString const &, bool)), tab_page, SIGNAL(signalFindPrevious(QString const &, bool)));
        }
        else
        {
            disconnect(ui.action_lock_zoom, SIGNAL(triggered()), tab_page, SLOT(slotZoomLock()));
            disconnect(ui.action_edit_zoom_in, SIGNAL(triggered()), tab_page, SLOT(slotZoomIn()));
            disconnect(ui.action_edit_zoom_out, SIGNAL(triggered()), tab_page, SLOT(slotZoomOut()));
            disconnect(ui.action_edit_zoom_reset, SIGNAL(triggered()), tab_page, SLOT(slotResetZoom()));
            disconnect(ui.action_edit_select_all, SIGNAL(triggered()), tab_page, SLOT(slotSelectAll()));
            disconnect(ui.action_edit_copy, SIGNAL(triggered()), tab_page, SLOT(slotCopy()));
            disconnect(ui.action_file_reload, SIGNAL(triggered()), tab_page, SLOT(slotReload()));
            disconnect(zoom_slider, SIGNAL(valueChanged(int)), tab_page, SLOT(slotSetZoom(int)));
            disconnect(tab_page, SIGNAL(signalScaleChanged()), this, SLOT(slotSynchronizeZoomSlider()));
            disconnect(find_dialog, SIGNAL(signalFindNext(QString const &, bool)), tab_page, SIGNAL(signalFindNext(QString const &, bool)));
            disconnect(find_dialog, SIGNAL(signalFindPrevious(QString const &, bool)), tab_page, SIGNAL(signalFindPrevious(QString const &, bool)));
        }
    }

    updateActionEnables();
    updateRecentFiles();

    // Wait until the event loop has finished rendering the current page.
    QTimer::singleShot(0, this, SLOT(slotSynchronizeZoomSlider()));
}

void TabWidget::openFiles(QStringList file_uris)
{
    if (file_uris.isEmpty())
        return;

    for (int i = 0; i < file_uris.size(); ++i)
        loadFile(file_uris[i]);

    TabPage *tab_page = uriTabPage(file_uris.last());
    setCurrentWidget(tab_page);
}

void TabWidget::slotCloseAllButCurrentTabPage()
{
    while (count() > 1)
    {
        int index_to_remove = (currentIndex() == 0) ? 1 : 0;
        slotRemoveTab(index_to_remove);
    }
}

void TabWidget::slotCloseAllTabPages()
{
    while (count())
        slotCloseCurrentTab();
}

void TabWidget::mouseMoveEvent(QMouseEvent *event)
{
    QTabBar *tab_bar = tabBar();
    // Update the status tip with the file uri if the user hovers over a tab.
    if (tab_bar && tab_bar->tabAt(event->pos()) != -1)
    {
        int tab_index = tab_bar->tabAt(event->pos());
        auto tab_widget = widget(tab_index);
        if (tab_widget)
        {
            assert(dynamic_cast<QMainWindow *>(parent()));
            auto *main_window = static_cast<QMainWindow *>(parent());
            main_window->statusBar()->showMessage(tabUri(tab_widget));
        }
    }

    return QTabWidget::mouseMoveEvent(event);
}

void TabWidget::slotOpenRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        slotOpenFile(action->text());
}

void TabWidget::slotRemoveTab(int tab_index)
{
    // When there are no tabs, the open dialog starts in lastOpenUri's directory.
    settings.setValue("lastOpenUri", currentTabUri());

    // Stop watching the file corresponding to this tab.
    file_watcher->removePath(tabUri(widget(tab_index)));

    auto *tab_page = tabPage(widget(tab_index));
    tab_page->slotSaveSettings();
    delete tab_page;
}

void TabWidget::slotCurrentTabChanged(int new_index)
{
    assert(count() > new_index);

    // If we have just closed the last tab.
    if (new_index != -1)
    {
        auto *tab_page = tabPage(widget(new_index));
        if (!tab_page->wasCurrentTab())
        {
            tab_page->setCurrentTab();

            // Wait until the event loop has finished rendering the current page.
            QTimer::singleShot(0, tab_page, SLOT(slotLoadSettings()));
        }
    }

    updateTabConnections();
}

void TabWidget::slotFileChanged(QString changed_file_uri)
{
    TabPage *tab_page = uriTabPage(changed_file_uri);

    // If the file has been deleted, remove it's tab if the user has specified tab removal.
    QFileInfo file_info(changed_file_uri);
    if (!file_info.exists())
    {
        if (ui.action_close_deleted_files->isChecked())
            slotRemoveTab(indexOf(tab_page));
        return;
    }

    setCurrentWidget(tab_page);

    // Give the application that is modifying this file a bit of time to finish.
    QTimer::singleShot(20, tab_page, SLOT(slotReload()));
}

void TabWidget::slotOpenFile(QString const &file_uri)
{
    TabPage *tab_page = loadFile(file_uri);
    if (tab_page)
        setCurrentWidget(tab_page);
}

void TabWidget::slotOpenFiles()
{
    // The open dialog starts in the current uri, or last open uri's directory.
    QString open_from_uri = currentTabUri();
    if (open_from_uri.isEmpty())
        open_from_uri = settings.value("lastOpenUri", "").toString();

    QStringList file_uris = QFileDialog::getOpenFileNames(
        this, tr("Select File"), open_from_uri, tr("*.*"));

    slotOpenFiles(file_uris);
}

void TabWidget::slotOpenFiles(QStringList const &file_uris)
{
    for (int i = 0; i < file_uris.count(); ++i)
        slotOpenFile(file_uris[i]);
}

void TabWidget::loadSettings()
{
    // We are blocking signals so that we can load in all the tabs before resizing and scrolling them.
    bool block_signals = blockSignals(true);

    QStringList document_uris = settings.value("open_tabs", QStringList()).toStringList();
    for (int i = 0; i < document_uris.count(); ++i)
        loadFile(document_uris[i]);

    QString current_uri = settings.value("current_tab").toString();
    auto *current_tab_page = uriTabPage(current_uri);
    if (current_tab_page)
        setCurrentWidget(current_tab_page);

    find_dialog->loadSettings();

    blockSignals(block_signals);

    slotCurrentTabChanged(currentIndex());
}

void TabWidget::saveSettings()
{
    settings.setValue("open_tabs", allTabUris());
    settings.setValue("current_tab", currentTabUri());

    auto pages = allTabPages();
    std::for_each(pages.begin(), pages.end(), [](TabPage *page)
                  { page->slotSaveSettings(); });

    find_dialog->saveSettings();
}

TabPage *TabWidget::loadFile(QString const &file_uri)
{
    TabPage *newTab = new TabPage(ui, this); // Create a new TabPage

    if (file_uri.isEmpty())
        return nullptr;

    QFileInfo file_info(file_uri);
    if (!file_info.exists())
        return nullptr;

    QString canonical_file_uri = file_info.canonicalFilePath();

    // If this file is already loaded, switch to that tab.
    TabPage *loaded_tab_page = uriTabPage(canonical_file_uri);
    if (loaded_tab_page)
    {
        setCurrentWidget(loaded_tab_page);
        delete newTab; // Delete the unused new tab
        return loaded_tab_page;
    }

    if (!newTab->load(canonical_file_uri))
    {
        QMessageBox::warning(this, tr("File Error"), tr("This file could not be loaded."),
                             QMessageBox::Ok, QMessageBox::NoButton);
        delete newTab;
        return nullptr;
    }

    newTab->makeBackgroundTransparent(ui.action_transparent_background->isChecked());
    newTab->wordWrap(ui.action_word_wrap->isChecked());
    newTab->indentXML(ui.action_indent_xml->isChecked());
    newTab->scrollToBottomOnChange(ui.action_scroll_to_bottom->isChecked());

    addTab(newTab, file_info.fileName());
    file_watcher->addPath(file_uri);

    return newTab;
}

TabPage *TabWidget::tabPage(QWidget *widget) const
{
    // TabWidget should only have TabPage child Widgets.
    assert(dynamic_cast<TabPage *>(widget));

    return static_cast<TabPage *>(widget);
}

std::vector<TabPage *> TabWidget::allTabPages() const
{
    std::vector<TabPage *> tab_pages;

    for (int i = 0; i < count(); ++i)
        tab_pages.push_back(tabPage(widget(i)));

    return tab_pages;
}

void TabWidget::slotCloseCurrentTab()
{
    slotRemoveTab(currentIndex());
}

void TabWidget::slotMakeBackgroundTransparent(bool transparent)
{
    auto pages = allTabPages();
    std::for_each(pages.begin(), pages.end(), [transparent](TabPage *page)
                  { page->makeBackgroundTransparent(transparent); });
}

void TabWidget::slotWordWrap(bool word_wrap)
{
    auto pages = allTabPages();
    std::for_each(pages.begin(), pages.end(), [word_wrap](TabPage *page)
                  { page->wordWrap(word_wrap); });
}

void TabWidget::slotIndentXML(bool indent_xml)
{
    auto pages = allTabPages();
    std::for_each(pages.begin(), pages.end(), [indent_xml](TabPage *page)
                  { page->indentXML(indent_xml); });
}

void TabWidget::slotScrollToBottomOnChange(bool scroll_to_bottom)
{
    auto pages = allTabPages();
    std::for_each(pages.begin(), pages.end(), [scroll_to_bottom](TabPage *page)
                  { page->scrollToBottomOnChange(scroll_to_bottom); });
}

TabPage *TabWidget::uriTabPage(QString const &uri) const
{
    for (int i = 0; i < count(); ++i)
        if (uri == tabUri(widget(i)))
            return tabPage(widget(i));

    return nullptr;
}

QString TabWidget::tabUri(QWidget *tab_widget) const
{
    return tabPage(tab_widget)->getUri();
}

QString TabWidget::currentTabUri() const
{
    if (count())
        return tabUri(currentWidget());

    return "";
}

QStringList TabWidget::allTabUris() const
{
    QStringList tab_page_uris;
    for (int i = 0; i < count(); ++i)
        tab_page_uris << tabUri(widget(i));

    return tab_page_uris;
}

// All recent files that are not open and are still on disk.
QStringList TabWidget::allRecentlyClosedFilesOnDisk() const
{
    // Grab a list of all the files that we have attributes saved for.
    QSettings global_settings;
    global_settings.beginGroup("files");
    QStringList file_uri_keys = global_settings.allKeys();

    // Each key has a delimiter prefix and attribute suffix.  Remove these non-uri elements.
    static QRegularExpression remove_file_attributes("(?:\\*)(.*)(?:\\/.*)");
    file_uri_keys.replaceInStrings(remove_file_attributes, "\\1");

    // Remove duplicate uris (each uri has multiple attributes).
    file_uri_keys.removeDuplicates();

    // Remove the tabs currently open.
    QStringList file_uris = file_uri_keys - allTabUris();

    // Remove any files that aren't on disk.
    return file::clearUrisWithNoFileOnDisk(file_uris);
}

void TabWidget::updateRecentFiles()
{
    ui.menu_file_open_recent->clear();

    QStringList recent_files = allRecentlyClosedFilesOnDisk();
    for (int i = 0; i < recent_files.count(); ++i)
    {
        auto *action = new QAction(recent_files.at(i), this);
        connect(action, SIGNAL(triggered()), this, SLOT(slotOpenRecentFile()));
        ui.menu_file_open_recent->addAction(action);
    }

    if (ui.menu_file_open_recent->isEmpty())
    {
        auto *recent_action = new QAction(tr("No Recent Files"), this);
        recent_action->setStatusTip(tr("No recently closed files that are on disk were found."));
        ui.menu_file_open_recent->addAction(recent_action);
    }
}

void TabWidget::updateActionEnables()
{
    bool open_files = (count() > 0);
    bool is_text = open_files ? tabPage(widget(currentIndex()))->isText() : false;

    ui.action_edit_copy->setEnabled(open_files && is_text);
    ui.action_edit_find->setEnabled(open_files && is_text);
    ui.action_edit_select_all->setEnabled(open_files && is_text);
    ui.action_edit_find->setEnabled(open_files && is_text);

    ui.action_file_close->setEnabled(open_files);
    ui.action_file_close_all->setEnabled(open_files);
    ui.action_session_save_as->setEnabled(open_files);

    ui.action_edit_zoom_in->setEnabled(open_files);
    ui.action_edit_zoom_out->setEnabled(open_files);
    ui.action_edit_zoom_reset->setEnabled(open_files);
    zoom_slider->setEnabled(open_files);

    ui.action_file_close_others->setEnabled(count() > 1);
}

// Update the zoom slider using the current tab's zoom value.
void TabWidget::slotSynchronizeZoomSlider()
{
    // This is called in a delayed manner and might be run when there are no more open tabs.
    if (!count())
        return;

    bool block_signals = zoom_slider->blockSignals(true);
    zoom_slider->setValue(tabPage(widget(currentIndex()))->getPercentageZoom());
    zoom_slider->blockSignals(block_signals);
}

void TabWidget::slotFind()
{
    find_dialog->show();
}
