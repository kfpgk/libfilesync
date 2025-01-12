#include <libfilesync/curl/storage/char_buffer/visitor/GetSpan.hpp>

namespace filesync::curl::storage::char_buffer::visitor {

    std::span<char> GetSpan::operator()(ReadBuffer& buffer) {
        return buffer.getSpan();
    }

    std::span<char> GetSpan::operator()(ReadWriteBuffer& buffer) {
        return buffer.getSpan();
    }

}