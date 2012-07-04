
#ifndef ABOUT_DIALOG_HPP
#define ABOUT_DIALOG_HPP

#include <QDialog>

#include <memory>

namespace Ui { class AboutDialog; }

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget * parent = 0);

private:
    std::shared_ptr<Ui::AboutDialog> ui;
};

#endif
