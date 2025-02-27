
#include "contextmenus.hpp"

#include "ui_mainwindow.h"

TextContextMenu::TextContextMenu(Ui::MainWindow const& ui) : QMenu(0), ui(ui)
{
    addAction(ui.action_edit_select_all);
    addAction(ui.action_edit_copy);
    addSeparator();
    addAction(ui.action_edit_find);
}

ImageContextMenu::ImageContextMenu(Ui::MainWindow const& ui) : QMenu(0), ui(ui)
{
    addAction(ui.action_edit_zoom_in);
    addAction(ui.action_edit_zoom_out);
    addSeparator();
    addAction(ui.action_edit_zoom_reset);
}
