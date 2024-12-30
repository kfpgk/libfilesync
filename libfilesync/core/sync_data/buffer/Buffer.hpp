#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_BUFFER_HPP

#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/protocol/memory/Handle.hpp>

#include <filesystem>
#include <variant>

namespace filesync::core::sync_data::buffer {

    using Buffer = std::variant<FileBuffer, ProtocolMemoryBuffer>;

    using HandleType = std::variant<
        std::filesystem::path,
        std::unique_ptr<protocol::memory::Handle<char>>>;

}

#endif