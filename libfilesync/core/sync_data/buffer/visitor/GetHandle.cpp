#include <libfilesync/core/sync_data/buffer/visitor/GetHandle.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    protocol::HandleOrFilePath GetHandle::operator()(const FileBuffer& buffer) {
        return buffer.getFilePath();
    }

    protocol::HandleOrFilePath GetHandle::operator()(ProtocolMemoryBuffer& buffer) {
        return buffer.getHandle();
    }

}