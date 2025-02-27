
#ifndef CONTEXT_MENUS_HPP
#define CONTEXT_MENUS_HPP

#include <QMenu>

namespace Ui
{
class MainWindow;
}

class TextContextMenu : public QMenu
{
   public:
    explicit TextContextMenu(Ui::MainWindow const& ui);

   private:
    Ui::MainWindow const& ui;
};

class ImageContextMenu : public QMenu
{
   public:
    explicit ImageContextMenu(Ui::MainWindow const& ui);

   private:
    Ui::MainWindow const& ui;
};

#endif
