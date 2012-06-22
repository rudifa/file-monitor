
#include "aboutdialog.hpp"

#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::AboutDialog())
{
    ui->setupUi(this);

    QString file_monitor_project_url("http://code.google.com/p/file-monitor");

    auto content = QString("<img src=\":/icons/file-monitor.xpm\" style=\"float: left; \"/>"
        "<p style=\"margin-left: 40px; margin-top: -20px;\">%1</p><p>%2</p><p>%3</p><p>%4</p><p>%5%6</p>")
    .arg(tr("<b>File Monitor</b> is a cross-platform file viewer that automatically redraws files when they change on disk."))
    .arg(tr("You can edit an HTML document in a simple text editor and watch it update in real-time using File Monitor."))
    .arg(tr("If you are keeping track of 10 different log files, File Monitor will automatically switch to the most recently "
           "modified file and take you to those changes."))
    .arg(tr("Currently supported file types are text, html, svg, and raster image file formats (jpg, gif, png, bmp, etc)."))
    .arg(tr("To learn more about File Monitor or to contribute, please visit our project page "))
    .arg("<a href=\"" + file_monitor_project_url + "\">" + file_monitor_project_url + "</a>");

    ui->content_label->setText(content);
}
