
#include "finddialog.hpp"

FindDialog::FindDialog(QWidget * parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.find_next_push_button, SIGNAL(clicked()), SLOT(slotFindNext()));
    connect(ui.find_previous_push_button, SIGNAL(clicked()), SLOT(slotFindPrevious()));

    settings.beginGroup("search");
}

void FindDialog::loadSettings()
{
    ui.find_combo_box->clear();

    QStringList previous_search_items = settings.value("previous").toStringList();
    if (previous_search_items.isEmpty())
        return;

    ui.find_combo_box->addItems(previous_search_items);
}

void FindDialog::saveSettings()
{
    settings.setValue("previous", previousSearchTerms());
}

void FindDialog::slotFindNext()
{
    QString search_string = ui.find_combo_box->currentText();
    signalFindNext(search_string, ui.match_case_checkbox->isChecked());

    // If the text has already been entered.
    int index = ui.find_combo_box->findText(search_string);
    if (index == -1)
        ui.find_combo_box->insertItem(index, search_string);
    else
        ui.find_combo_box->setCurrentIndex(index);
}

void FindDialog::slotFindPrevious()
{
    QString search_string = ui.find_combo_box->currentText();
    signalFindPrevious(search_string, ui.match_case_checkbox->isChecked());

    // If the text has already been entered.
    int index = ui.find_combo_box->findText(search_string);
    if (index == -1)
        ui.find_combo_box->insertItem(index, search_string);
    else
        ui.find_combo_box->setCurrentIndex(index);
}

QStringList FindDialog::previousSearchTerms() const
{
    QStringList previous_search_terms;
    for (int i = 0; i < ui.find_combo_box->count(); ++i)
        previous_search_terms << ui.find_combo_box->itemText(i);

    return previous_search_terms;
}
