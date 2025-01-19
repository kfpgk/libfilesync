#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_PROTOCOL_MEMORY_BUFFER_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_PROTOCOL_MEMORY_BUFFER_HPP

#include <libfilesync/protocol/memory/Handle.hpp>

#include <filesystem>
#include <istream>
#include <memory>

namespace filesync::core::sync_data::buffer {

    /**
     * @brief Buffer that stores data in memory
     * and is compatible to the protocol package.
     * 
     * It can be handed to protocol instances for
     * uploading from and downloading into this 
     * buffer. It cannot be written manually. 
     * 
     * Patterns:
     * 
     *  - Element of visitor pattern
     */
    class ProtocolMemoryBuffer {

        public:
            ProtocolMemoryBuffer() = default;
            explicit ProtocolMemoryBuffer(std::unique_ptr<protocol::memory::Handle<char>> data);
            ProtocolMemoryBuffer(const ProtocolMemoryBuffer& rhs);
            ProtocolMemoryBuffer(ProtocolMemoryBuffer&& rhs) noexcept;
            ProtocolMemoryBuffer& operator=(ProtocolMemoryBuffer rhs);
            ~ProtocolMemoryBuffer() = default;
            friend void swap(ProtocolMemoryBuffer& lhs, ProtocolMemoryBuffer& rhs) noexcept;

            std::unique_ptr<ProtocolMemoryBuffer> cloneType() const;
            [[nodiscard]] std::unique_ptr<protocol::memory::Handle<char>>& getHandle();
            void store(std::unique_ptr<protocol::memory::Handle<char>> in);
            std::ostream& extractContentTo(std::ostream& out);
            bool isEqualTo(std::istream& in) const;

        private:
            std::unique_ptr<protocol::memory::Handle<char>> data;

            std::streampos getInStreamSize(std::istream& in) const;
            
    };

}

#endif