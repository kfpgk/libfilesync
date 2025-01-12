#include <libfilesync/curl/storage/char_buffer/visitor/ResetPosition.hpp>

namespace filesync::curl::storage::char_buffer::visitor {

    void ResetPosition::operator()(ReadBuffer& buffer) {
        buffer.resetPosition();
    }

    void ResetPosition::operator()(ReadWriteBuffer& buffer) {
        buffer.resetPosition();
    }

}