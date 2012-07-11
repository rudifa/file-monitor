
#ifndef FIND_DIALOG_HPP
#define FIND_DIALOG_HPP

#include "ui_finddialog.h"

#include <QDialog>
#include <QSettings>

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget * parent = 0);
    void loadSettings();
    void saveSettings();
    
signals:
    void signalFindNext(QString const & text, bool case_sensitive);
    void signalFindPrevious(QString const & text, bool case_sensitive);

private slots:
    void slotFindNext();
    void slotFindPrevious();

private:
    Ui::FindDialog ui;
    QSettings settings;

    QStringList previousSearchTerms() const;
};

#endif
