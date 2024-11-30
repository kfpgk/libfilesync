#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_WRITE_CONTENT_TO_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_WRITE_CONTENT_TO_HPP

#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/MemoryBuffer.hpp>

#include <ostream>

namespace filesync::core::sync_data::buffer::visitor {

    /**
     * @brief Callable object to write buffer content
     * to an outstream.
     * 
     * Patterns:
     *  - Visitor
     */
    class WriteContentTo {

        public:
            WriteContentTo(std::ostream& out);

            bool operator()(FileBuffer& buffer);
            bool operator()(MemoryBuffer& buffer);

        private:
            std::ostream& out;

    };

}

#endif 