#include <libfilesync/core/sync_data/buffer/visitor/GetHandle.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    HandleType GetHandle::operator()(const FileBuffer& buffer) {
        return buffer.getFilePath();
    }

    HandleType GetHandle::operator()(ProtocolMemoryBuffer& buffer) {
        return buffer.getHandle();
    }

}