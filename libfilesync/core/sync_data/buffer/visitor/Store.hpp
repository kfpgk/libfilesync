#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_STORE_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_STORE_HPP

#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/MemoryBuffer.hpp>

#include <istream>

namespace filesync::core::sync_data::buffer::visitor {

    /**
     * @brief Callable object to store stream into
     * a buffer.
     * 
     * Patterns:
     *  - Visitor
     */
    class Store {

        public:
            Store(std::istream& in);

            void operator()(FileBuffer& buffer);
            void operator()(MemoryBuffer& buffer);

        private:
            std::istream& in;

    };

}

#endif 