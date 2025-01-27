#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_WRITE_CONTENT_TO_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_WRITE_CONTENT_TO_HPP

#include <libfilesync/core/sync_data/buffer/CharArrayMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>

#include <memory>
#include <ostream>

namespace filesync::core::sync_data::buffer::visitor {

    /**
     * @brief Callable object to write buffer content
     * to an outstream.
     * 
     * Patterns:
     * 
     *  - Visitor
     */
    class WriteContentTo {

        public:
            explicit WriteContentTo(std::ostream& out);

            std::ostream& operator()(std::unique_ptr<CharArrayMemoryBuffer>& buffer);
            std::ostream& operator()(std::unique_ptr<FileBuffer>& buffer);
            std::ostream& operator()(std::unique_ptr<ProtocolMemoryBuffer>& buffer);

        private:
            std::ostream& out;

    };

}

#endif 