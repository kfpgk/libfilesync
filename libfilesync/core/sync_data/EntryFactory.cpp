#include <libfilesync/core/sync_data/EntryFactory.hpp>
#include <libfilesync/core/sync_data/Directory.hpp>
#include <libfilesync/core/sync_data/File.hpp>
#include <libfilesync/data/EntryFactory.hpp>
#include <libfilesync/data/Exception.hpp>

#include <libfilesync/utility/Debug.hpp>

#include <string>
#include <fstream>

namespace filesync::core::sync_data {

    /**
     * @brief Build a SyncEntry data structure
     * based on file system path
     * 
     * Create composite SyncEntry object including
     * all files and directories in "path" argument.
     * 
     * If "path" is a single file, return a SyncEntry
     * object that represents that single file.
     */
    std::unique_ptr<Entry> createSyncEntryRecursively(
        const std::filesystem::path& path) {

        std::filesystem::path normalizedPath = path;
        normalizedPath.make_preferred();

        std::unique_ptr<Entry> entry; 
        entry = filesync::data::createEntryRecursively<
            Entry, File, Directory>(path);
        
        if (path.string().back() == std::filesystem::path::preferred_separator) {
            entry->setRemoteEntry(normalizedPath.parent_path().filename());
        } else {
            entry->setRemoteEntry(normalizedPath.filename());
        }

        return std::move(entry);
    }

}