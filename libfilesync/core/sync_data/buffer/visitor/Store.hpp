#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_STORE_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_STORE_HPP

#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/protocol/memory/Handle.hpp>

#include <functional>
#include <istream>
#include <variant>

namespace filesync::core::sync_data::buffer::visitor {

    using BufferStoreType = std::variant<
        std::reference_wrapper<std::istream>,
        std::unique_ptr<protocol::memory::Handle<char>>>; 

    /**
     * @brief Callable object to store stream into
     * a buffer.
     * 
     * Patterns:
     *  - Visitor
     */
    class Store {

        public:
            explicit Store(BufferStoreType in);

            void operator()(FileBuffer& buffer);
            void operator()(ProtocolMemoryBuffer& buffer);

        private:
            BufferStoreType in;

    };

}

#endif 