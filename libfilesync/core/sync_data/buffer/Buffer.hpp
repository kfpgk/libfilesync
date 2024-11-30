#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_BUFFER_HPP

#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/MemoryBuffer.hpp>

#include <filesystem>
#include <variant>

namespace filesync::core::sync_data::buffer {

    using Buffer = std::variant<FileBuffer, MemoryBuffer>;

    using DataLocation = std::variant<std::filesystem::path, void*>;

}

#endif