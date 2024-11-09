#include <libfilesync/core/sync_data/File.hpp>

namespace filesync::core::sync_data {

    File::File(const std::filesystem::path& path) :
        Entry{path},
        FileBase{path},
        EntryBase{path} {

    }

}