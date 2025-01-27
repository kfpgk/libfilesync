#include <libfilesync/core/sync_data/buffer/visitor/GetHandle.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    std::span<char> GetHandle::operator()(
        std::unique_ptr<CharArrayMemoryBuffer>& buffer) {
        return buffer->getData();
    }

    protocol::HandleOrFilePath GetHandle::operator()(
        const std::unique_ptr<FileBuffer>& buffer) {
        return buffer->getFilePath();
    }

    protocol::HandleOrFilePath GetHandle::operator()(
        std::unique_ptr<ProtocolMemoryBuffer>& buffer) {
        return buffer->getHandle();
    }

}