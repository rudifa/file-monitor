
#include "tabwidget.hpp"

#include "ui_mainwindow.h"
#include "tabpage.hpp"

#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTimer>
#include <QSlider>
#include <QMessageBox>

#include <cassert>

TabWidget::TabWidget(Ui::MainWindow * ui, QWidget * parent)
    : QTabWidget(parent), ui(ui), zoom_slider(new QSlider(Qt::Horizontal, parent))
{
    setDocumentMode(true);
    setMovable(true);
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(slotRemoveTab(int)));
    connect(this, SIGNAL(currentChanged(int)), SLOT(slotCurrentTabChanged()));

    zoom_slider->setToolTip(tr("Zoom Slider"));
    zoom_slider->setStatusTip(tr("Drag slider to zoom into or out of the current file."));
    zoom_slider->setMaximumWidth(400);
    zoom_slider->setRange(TabPage::zoom_min, TabPage::zoom_max);
    zoom_slider->setSingleStep(TabPage::zoom_step);
    ui->tool_bar->insertWidget(ui->action_file_zoom_in, zoom_slider);

    // Load each of the documents from the last session.
    QStringList document_uris = settings.value("tabUris", QStringList()).toStringList();
    for (int i = 0; i < document_uris.count(); ++i)
        loadFile(document_uris[i]);

    setCurrentIndex(settings.value("currentTabIndex", -1).toInt());
}

TabWidget::~TabWidget()
{
    // Save open document uris so that they appear the next session.
    settings.setValue("tabUris", allTabUris());
    settings.setValue("currentTabIndex", currentIndex());
}

void TabWidget::updateTabConnections()
{
    // Update tab connections.
    for (int i = 0; i < count(); ++i) {
        TabPage * tab_page = dynamic_cast<TabPage *>(widget(i));
        assert(tab_page);
        if (i == currentIndex())
        {
            connect(ui->action_file_zoom_in, SIGNAL(triggered()), tab_page, SLOT(slotZoomIn()));
            connect(ui->action_file_zoom_out, SIGNAL(triggered()), tab_page, SLOT(slotZoomOut()));
            connect(zoom_slider, SIGNAL(valueChanged(int)), tab_page, SLOT(slotSetZoom(int)));
            connect(tab_page, SIGNAL(signalUserChangedZoom(int)), zoom_slider, SLOT(setValue(int)));
        }
        else
        {
            disconnect(ui->action_file_zoom_in, SIGNAL(triggered()), tab_page, SLOT(slotZoomIn()));
            disconnect(ui->action_file_zoom_out, SIGNAL(triggered()), tab_page, SLOT(slotZoomOut()));
            disconnect(zoom_slider, SIGNAL(valueChanged(int)), tab_page, SLOT(slotSetZoom(int)));
            disconnect(tab_page, SIGNAL(signalUserChangedZoom(int)), zoom_slider, SLOT(setValue(int)));
        }
    }

    // Wait until the event loop has finished rendering the current page.
    QTimer::singleShot(0, this, SLOT(slotSynchronizeZoomSlider()));
}

void TabWidget::openFiles(QStringList file_uris)
{
    if (file_uris.isEmpty())
        return;

    for (int i = 0; i < file_uris.size(); ++i)
        loadFile(file_uris[i]);

    TabPage * tab_page = uriTabPage(file_uris.last());
    setCurrentWidget(tab_page);
}

void TabWidget::slotRemoveTab(int index)
{
    // When there are no tabs, the open dialog starts in lastOpenUri's directory.
    settings.setValue("lastOpenUri", tabUri(currentWidget()));

    delete widget(index);
}

void TabWidget::slotCurrentTabChanged()
{
    // If we have just closed the last tab.
    if (!count())
        return;

    TabPage * tab_page = dynamic_cast<TabPage *>(currentWidget());
    // Wait until the event loop has finished rendering the current page.
    QTimer::singleShot(0, tab_page, SLOT(slotLoadSettings()));

    updateTabConnections();
}

void TabWidget::slotFileChanged(QString changed_file_uri)
{
    TabPage * tab_page = uriTabPage(changed_file_uri);
    setCurrentWidget(tab_page);

    // Give the application that is modifying this file a bit of time to finish.
    QTimer::singleShot(20, tab_page, SLOT(slotReload()));
}

void TabWidget::slotLoadFile()
{
    // The open dialog starts in the current uri, or last open uri's directory.
    QString open_from_uri = tabUri(currentWidget());
    if (open_from_uri.isEmpty())
        open_from_uri = settings.value("lastOpenUri", "").toString();

    QString file_uri = QFileDialog::getOpenFileName(
        this, tr("Select File"), open_from_uri, tr("*.*"));

    TabPage * tab_page = loadFile(file_uri);
    if (tab_page)
        setCurrentWidget(tab_page);
}

TabPage * TabWidget::loadFile(QString const & file_uri)
{
    if (file_uri.isEmpty())
        return 0;

    QFileInfo file_info(file_uri);
    if (!file_info.exists())
        return 0;

    QString canonical_file_uri = file_info.canonicalFilePath();

    // If this file is already loaded, switch to that tab.
    TabPage * loaded_tab_page = uriTabPage(canonical_file_uri);
    if (loaded_tab_page)
    {
        setCurrentWidget(loaded_tab_page);
        return loaded_tab_page;
    }

    TabPage * tab_page = new TabPage(this);
    if (!tab_page->load(canonical_file_uri))
    {
        QMessageBox::warning(this, tr("File Error"), tr("This file could not be loaded."),
            QMessageBox::Ok, QMessageBox::NoButton);
        tab_page->deleteLater();
        return 0;
    }

    addTab(tab_page, file_info.fileName());
    connect(tab_page->getFileWatcher(), SIGNAL(fileChanged(QString)), SLOT(slotFileChanged(QString)));
    return tab_page;
}

void TabWidget::slotCloseCurrentTab()
{
    slotRemoveTab(currentIndex());
}

TabPage * TabWidget::uriTabPage(QString const & uri) const
{
    for (int i = 0; i < count(); ++i)
        if (uri == tabUri(widget(i)))
            return dynamic_cast<TabPage *>(widget(i));

    return 0;
}

QString TabWidget::tabUri(QWidget * tab_widget) const
{
    // If no tab widget was supplied, return empty string.
    if (!tab_widget)
        return "";

    // If we were given a tab_widget, it better be a TabPage.
    TabPage * tab_page = dynamic_cast<TabPage *>(tab_widget);
    assert(tab_page);

    return tab_page->getUri();
}

QStringList TabWidget::allTabUris() const
{
    QStringList tab_page_uris;
    for (int i = 0; i < count(); ++i)
        tab_page_uris << tabUri(widget(i));

    return tab_page_uris;
}

// Update the zoom slider using the current tab's zoom value.
void TabWidget::slotSynchronizeZoomSlider()
{
    TabPage * tab_page = dynamic_cast<TabPage *>(widget(currentIndex()));
    assert(tab_page);

    zoom_slider->blockSignals(true);
    zoom_slider->setValue(tab_page->getZoom());
    zoom_slider->blockSignals(false);
}
