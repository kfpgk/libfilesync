#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_BUFFER_HPP

#include <libfilesync/core/sync_data/buffer/CharArrayMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>

#include <memory>
#include <variant>

namespace filesync::core::sync_data::buffer {

    using Buffer = std::variant<
        std::unique_ptr<CharArrayMemoryBuffer>,
        std::unique_ptr<FileBuffer>,
        std::unique_ptr<ProtocolMemoryBuffer>>;

}

#endif