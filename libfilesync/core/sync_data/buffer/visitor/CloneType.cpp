#include <libfilesync/core/sync_data/buffer/visitor/CloneType.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    Buffer CloneType::operator()(const std::unique_ptr<CharArrayMemoryBuffer>& buffer) {
        return buffer->cloneType();
    }

    Buffer CloneType::operator()(const std::unique_ptr<FileBuffer>& buffer) {
        return buffer->cloneType();
    }

    Buffer CloneType::operator()(const std::unique_ptr<ProtocolMemoryBuffer>& buffer) {
        return buffer->cloneType();
    }

}