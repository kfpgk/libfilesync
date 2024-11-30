#include <libfilesync/core/sync_data/buffer/visitor/GetLocation.hpp>

namespace filesync::core::sync_data::buffer::visitor {

    DataLocation GetLocation::operator()(const FileBuffer& buffer) {
        return buffer.getFilePath();
    }

    DataLocation GetLocation::operator()(const MemoryBuffer& buffer) {
        return buffer.getData();
    }

}