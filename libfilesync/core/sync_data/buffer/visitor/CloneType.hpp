#ifndef LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_CLONE_TYPE_HPP
#define LIBFILESYNC_CORE_SYNC_DATA_BUFFER_VISITOR_CLONE_TYPE_HPP

#include <libfilesync/core/sync_data/buffer/CharArrayMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/Buffer.hpp>

#include <memory>

namespace filesync::core::sync_data::buffer::visitor {

    class CloneType {

        public:
            Buffer operator()(const std::unique_ptr<CharArrayMemoryBuffer>& buffer);
            Buffer operator()(const std::unique_ptr<FileBuffer>& buffer);
            Buffer operator()(const std::unique_ptr<ProtocolMemoryBuffer>& buffer);
    };

}

#endif