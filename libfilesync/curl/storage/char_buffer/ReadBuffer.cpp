#include <libfilesync/curl/storage/char_buffer/ReadBuffer.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/utility/Debug.hpp>

#include <cstring>
#include <utility>

namespace filesync::curl::storage::char_buffer {

    ReadBuffer::ReadBuffer(char* data, std::size_t dataSize) :
        data{data},
        dataSize{dataSize},
        position{this->data} {

    }

    ReadBuffer::ReadBuffer(std::string& data) :
        ReadBuffer(data.data(), data.size()) {
    }

    ReadBuffer::ReadBuffer(std::span<char> data) :
        ReadBuffer(data.data(), data.size()) {
    }

    ReadBuffer::ReadBuffer(ReadBuffer& data) :
        ReadBuffer(data.getSpan()) {
    }

    ReadBuffer::ReadBuffer(const ReadBuffer& rhs) {
        dataSize = rhs.dataSize;
        data = rhs.data;
    }

    ReadBuffer::ReadBuffer(ReadBuffer&& rhs) noexcept {
        using std::swap;
        swap(*this, rhs);
    }

    ReadBuffer& ReadBuffer::operator=(ReadBuffer rhs) {
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    void swap(ReadBuffer& lhs, ReadBuffer& rhs) noexcept {
        lhs.data = std::exchange(rhs.data, lhs.data);
        lhs.dataSize = std::exchange(rhs.dataSize, lhs.dataSize);
        lhs.position = std::exchange(rhs.position, lhs.position);
    }

    void ReadBuffer::resetPosition() {
        position = data;
    }

    void ReadBuffer::print() {
        std::cout << "-- CharBuffer --" << std::endl;
        std::cout << "Size: " << dataSize << std::endl;
        std::cout << "Data:" << std::endl;
        for(int i = 0; i < dataSize; i++) {
            std::cout << data[i];
        }
        std::cout << std::endl;
        std::cout << "------------------" << std::endl;
    }

    std::size_t ReadBuffer::getSize() const {
        return dataSize;
    }

    std::size_t ReadBuffer::read(char* buffer, std::size_t bufferSize) {
        std::size_t readSize = (&(data[dataSize]) - position) * sizeof(char);
        if (readSize > bufferSize) {
            readSize = bufferSize;
        }
        std::memcpy(buffer, position, readSize);
        position += readSize / sizeof(char);

        return readSize;
    }

    std::span<char> ReadBuffer::getSpan() {
        return std::span{data, dataSize};
    }

    std::string ReadBuffer::getString() const {
        return std::string{data, dataSize};
    }

    bool operator==(const ReadBuffer& lhs, const ReadBuffer& rhs) {
        if (lhs.dataSize != rhs.dataSize) {
            return false;
        }
        for (int i; i < lhs.dataSize; i++) {
            if (lhs.data[i] != rhs.data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const ReadBuffer& lhs, const ReadBuffer& rhs) {
        return !(lhs== rhs);
    }

}