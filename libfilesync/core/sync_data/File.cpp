#include <libfilesync/core/sync_data/File.hpp>

namespace filesync::core::sync_data {

    File::File(const std::filesystem::path& path) :
        FileBase{path},
        Entry{path},
        EntryBase{path} {

    }

    File::File(const std::filesystem::path& path,
        buffer::Buffer&& bufferForRemote,
        buffer::Buffer&& bufferForPrevious) :
            FileBase{path},
            Entry{path, std::move(bufferForRemote), std::move(bufferForPrevious)},
            EntryBase{path} {

    }  

}