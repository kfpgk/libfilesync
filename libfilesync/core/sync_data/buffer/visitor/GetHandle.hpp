#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_GET_HANDLE_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_GET_HANDLE_HPP

#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>

#include <libfilesync/protocol/HandleOrFilePath.hpp>

#include <filesystem>
#include <variant>

namespace filesync::core::sync_data::buffer::visitor {

    /**
     * @brief Callable object to get a buffer handle or
     * filesystem path
     * 
     * Patterns:
     * 
     *  - Visitor
     */
    class GetHandle {

        public:
            [[nodiscard]] protocol::HandleOrFilePath operator()(const FileBuffer& buffer);
            [[nodiscard]] protocol::HandleOrFilePath operator()(ProtocolMemoryBuffer& buffer);

    };

}

#endif