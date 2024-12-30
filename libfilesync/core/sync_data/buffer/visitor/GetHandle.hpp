#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_GET_HANDLE_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_GET_HANDLE_HPP

#include <libfilesync/core/sync_data/buffer/Buffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>

#include <filesystem>
#include <variant>

namespace filesync::core::sync_data::buffer::visitor {

    /**
     * @brief Callable object to get an owning handle
     * (or a filepath).
     * 
     * Patterns:
     *  - Visitor
     */
    class GetHandle {

        public:
            [[nodiscard]] HandleType operator()(const FileBuffer& buffer);
            [[nodiscard]] HandleType operator()(ProtocolMemoryBuffer& buffer);

    };

}

#endif