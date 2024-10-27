#ifndef LIBFILESYNC_DATA_HPP
#define LIBFILESYNC_DATA_HPP

#include <libfilesync/data/Entry.hpp>

#include <filesystem>
#include <memory>

namespace filesync {

    namespace data {

        enum class EntryType { none, file, dir };

        void createFile(const std::filesystem::path& path);

        std::unique_ptr<Entry> createEntryRecursively(
            const std::filesystem::path& path);

    }

}

#endif