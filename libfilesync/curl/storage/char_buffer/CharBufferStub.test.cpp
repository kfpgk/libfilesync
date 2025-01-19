#include <libfilesync/curl/storage/char_buffer/CharBufferStub.test.hpp>

namespace filesync::curl::storage::char_buffer::unit_test {

    CharBufferStub::CharBufferStub(const std::string& data) :
        data{data} {

    }

    void CharBufferStub::resetPosition() {

    }

    std::span<char> CharBufferStub::getSpan() {
        return std::span<char>{data.data(), data.size()};
    }

}