#include <libfilesync/protocol/memory/CharArray.hpp>

namespace filesync::protocol::memory {

    CharArray::CharArray(std::vector<char>& data) :
        dataContainer(data) {

    }

    CharArray::CharArray(std::vector<char>&& data) :
        dataContainer(std::move(data)) {

    }

    std::span<char> CharArray::data() {
        return std::span<char>(dataContainer);
    }

}