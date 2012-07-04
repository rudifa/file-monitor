
#ifndef FILE_SYSTEM_WATCHER_HPP
#define FILE_SYSTEM_WATCHER_HPP

#include <QFileSystemWatcher>
#include <QFileInfo>

#include <vector>

class QTimer;
class QFileInfo;

// This class keeps track of deleted files and if these deleted
//  files are restored, it starts to watch them once again.
class FileSystemWatcher : public QFileSystemWatcher
{
    Q_OBJECT

public:
    explicit FileSystemWatcher(QObject * parent = 0);

private slots:
    void slotFileChanged(QString);
    void slotExamineDeletedFiles();

private:
    QTimer * deleted_files_timer;
    std::vector<QString> deleted_files;
    QFileInfo * file_info;
};

#endif
