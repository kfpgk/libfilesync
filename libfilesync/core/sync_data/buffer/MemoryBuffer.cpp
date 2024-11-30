#include <libfilesync/core/sync_data/buffer/MemoryBuffer.hpp>

namespace filesync::core::sync_data::buffer {

    MemoryBuffer::MemoryBuffer() {

    }

    void* MemoryBuffer::getData() const {
        return nullptr;
    }

    void MemoryBuffer::store(std::istream& in) {

    }

    bool MemoryBuffer::extractContentTo(std::ostream& out) {
        return false;
    }

    bool MemoryBuffer::isEqualTo(std::istream& in) const {
        return false;
    }
    
}