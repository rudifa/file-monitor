
#include "filesystemwatcher.hpp"

#include <QTimer>

#include <algorithm>

FileSystemWatcher::FileSystemWatcher(QObject * parent) :
    QFileSystemWatcher(parent), deleted_files_timer(new QTimer(this)),
    file_info(new QFileInfo())
{
    file_info->setCaching(false);

    connect(this, SIGNAL(fileChanged(QString)), SLOT(slotFileChanged(QString)));
    connect(deleted_files_timer, SIGNAL(timeout()), SLOT(slotExamineDeletedFiles()));
}

void FileSystemWatcher::slotFileChanged(QString file_uri)
{
    QFileInfo file_info(file_uri);
    if (!file_info.exists())
    {
        if (deleted_files.empty())
            deleted_files_timer->start(500);

        deleted_files.push_back(file_uri);
        removePath(file_uri);
    }
}

namespace
{
    struct OnDisk
    {
        OnDisk(QFileInfo * file_info) : file_info(file_info) { }
        bool operator () (QString const & file_uri)
        {
            file_info->setFile(file_uri);
            return file_info->exists();
        }
        QFileInfo * file_info;
    };
}

void FileSystemWatcher::slotExamineDeletedFiles()
{
    // If any of the deleted files have been restored, start watching them again.
    for (std::size_t i = 0; i < deleted_files.size(); ++i)
    {
        QString file_uri = deleted_files[i];
        file_info->setFile(file_uri);
        if (file_info->exists())
            addPath(file_uri);
    }

    deleted_files.erase(
        std::remove_if(deleted_files.begin(), deleted_files.end(), OnDisk(file_info)),
        deleted_files.end());

    if (deleted_files.empty())
        deleted_files_timer->stop();
}
