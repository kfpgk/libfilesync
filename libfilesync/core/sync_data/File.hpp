#ifndef LIBFILESYNC_CORE_SYNC_DATA_FILE_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_FILE_HPP

#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/data/File.hpp>

#include <filesystem>

namespace filesync::core::sync_data {

    class File : public Entry, public filesync::data::FileBase<Entry> {

        public:
            explicit File(const std::filesystem::path& path);

    };

}

#endif