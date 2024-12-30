#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_PROTOCOL_MEMORY_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_PROTOCOL_MEMORY_BUFFER_HPP

#include <libfilesync/protocol/memory/Handle.hpp>

#include <filesystem>
#include <istream>
#include <memory>

namespace filesync::core::sync_data::buffer {

    /**
     * @brief Buffer that stores data in memory.
     * 
     * Patterns:
     *  - Element of visitor pattern
     */
    class ProtocolMemoryBuffer {

        public:
            ProtocolMemoryBuffer() = default;
            explicit ProtocolMemoryBuffer(std::unique_ptr<protocol::memory::Handle<char>> data);

            [[nodiscard]] std::unique_ptr<protocol::memory::Handle<char>> getHandle();
            void store(std::unique_ptr<protocol::memory::Handle<char>> in);
            bool extractContentTo(std::ostream& out);
            bool isEqualTo(std::istream& in) const;

        private:
            std::unique_ptr<protocol::memory::Handle<char>> data;

            std::streampos getInStreamSize(std::istream& in) const;
            
    };

}

#endif