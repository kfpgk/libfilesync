#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_IS_EQUAL_TO_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_IS_EQUAL_TO_HPP

#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>

#include <istream>

namespace filesync::core::sync_data::buffer::visitor {

    /**
     * @brief Callable object to compare the buffer
     * against an input stream.
     * 
     * Patterns:
     *  - Visitor
     */
    class IsEqualTo {

        public:
            explicit IsEqualTo(std::istream& in);

            bool operator()(const FileBuffer& buffer);
            bool operator()(const ProtocolMemoryBuffer& buffer);

        private:
            std::istream& in;

    };

}

#endif 