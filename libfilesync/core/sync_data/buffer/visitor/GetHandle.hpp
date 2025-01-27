#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_GET_HANDLE_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_GET_HANDLE_HPP

#include <libfilesync/core/sync_data/buffer/CharArrayMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/protocol/HandleOrFilePath.hpp>

#include <filesystem>
#include <memory>
#include <span>
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
            [[nodiscard]] std::span<char> operator()(
                std::unique_ptr<CharArrayMemoryBuffer>& buffer);
            [[nodiscard]] protocol::HandleOrFilePath operator()(const 
            std::unique_ptr<FileBuffer>& buffer);
            [[nodiscard]] protocol::HandleOrFilePath operator()(
                std::unique_ptr<ProtocolMemoryBuffer>& buffer);

    };

}

#endif