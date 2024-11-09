#include <libfilesync/data/EntryFactory.hpp>
#include <libfilesync/data/File.hpp>
#include <libfilesync/data/Directory.hpp>

namespace filesync::data {

    std::unique_ptr<Entry> createEntryRecursively(
        const std::filesystem::path& path) {

        return std::move(createEntryRecursively<Entry, File, Directory>(path));
    } 

}