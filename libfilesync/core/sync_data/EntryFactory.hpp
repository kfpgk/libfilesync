#ifndef LIBFILESYNC_CORE_SYNC_DATA_ENTRY_FACTORY_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_ENTRY_FACTORY_HPP

#include <libfilesync/core/sync_data/Entry.hpp>

#include <filesystem>
#include <memory>

namespace filesync::core::sync_data {

    std::unique_ptr<Entry> createSyncEntryRecursively(
        const std::filesystem::path& path);

}

#endif