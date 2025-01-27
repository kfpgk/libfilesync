#ifndef LIBFILESYNC_DATA_ENTRY_FACTORY_HPP
#define LIBFILESYNC_DATA_ENTRY_FACTORY_HPP

#include <libfilesync/data/Entry.hpp>

namespace filesync::data {

    template<typename Entry, typename File, typename Directory>
    std::unique_ptr<Entry> createEntryRecursively(
        const std::filesystem::path& path);

    /**
     * @brief Build an Entry data structure
     * based on file system path
     * 
     * Create composite Entry object including
     * all files and directories in "path" argument.
     * 
     * If "path" is a single file, return an Entry
     * object that represents that single file.
     */
    std::unique_ptr<Entry> createEntryRecursively(
        const std::filesystem::path& path);

}

#include <libfilesync/data/EntryFactory.tpp>

#endif