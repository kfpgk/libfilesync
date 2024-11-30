#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_GET_LOCATION_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_GET_LOCATION_HPP

#include <libfilesync/core/sync_data/buffer/Buffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/MemoryBuffer.hpp>

#include <filesystem>
#include <variant>

namespace filesync::core::sync_data::buffer::visitor {

    /**
     * @brief Callable object to get location of a buffer's
     * data.
     * 
     * Patterns:
     *  - Visitor
     */
    class GetLocation {

        public:
            [[nodiscard]] DataLocation operator()(const FileBuffer& buffer);
            [[nodiscard]] DataLocation operator()(const MemoryBuffer& buffer);

    };

}

#endif