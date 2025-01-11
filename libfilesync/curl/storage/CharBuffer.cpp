#include <libfilesync/curl/storage/CharBuffer.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/utility/Debug.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>

namespace filesync::curl::storage {

    CharBuffer::CharBuffer() {

        clear();
    }

    CharBuffer::CharBuffer(std::size_t size) {

        changeBufferSize(size);
    }

    CharBuffer::CharBuffer(std::string data) {

        clear();
        write(data);
    }

    CharBuffer::CharBuffer(char* data, std::size_t dataSize) {

        clear();

        std::size_t remaining = dataSize;
        char* dataPtr = data;
        while (remaining > 0) {
            std::size_t written = write(dataPtr, remaining);
            remaining -= written;
            dataPtr += written;
        }
    }

    CharBuffer::CharBuffer(std::span<char>& data) {

        clear();

        std::size_t remaining = data.size();
        char* dataPtr = data.data();
        while (remaining > 0) {
            std::size_t written = write(dataPtr, remaining);
            remaining -= written;
            dataPtr += written;
        }

    }

    CharBuffer::CharBuffer(const CharBuffer& rhs) {
        changeBufferSize(rhs.dataSize);
        std::size_t remaining = rhs.dataSize;
        char* dataPtr = rhs.data;
        while (remaining > 0) {
            std::size_t written = write(dataPtr, remaining);
            remaining -= written;
            dataPtr += written;
        }
    }

    CharBuffer::CharBuffer(CharBuffer&& rhs) noexcept {
        using std::swap;
        swap(*this, rhs);
    }

    CharBuffer& CharBuffer::operator=(CharBuffer rhs) {
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    CharBuffer::~CharBuffer() {
        std::free(data);
    }

    void swap(CharBuffer& lhs, CharBuffer& rhs) noexcept {
        lhs.data = std::exchange(rhs.data, lhs.data);
        lhs.dataSize = std::exchange(rhs.dataSize, lhs.dataSize);
        lhs.bufferSize = std::exchange(rhs.bufferSize, lhs.bufferSize);
        lhs.position = std::exchange(rhs.position, lhs.position);
        lhs.reallocations = std::exchange(rhs.reallocations, lhs.reallocations);
    }

    void CharBuffer::resetPosition() {
        position = data;
    }

    void CharBuffer::print() {
        std::cout << "-- CharBuffer --" << std::endl;
        std::cout << "Size: " << dataSize << std::endl;
        std::cout << "Data:" << std::endl;
        for(int i = 0; i < dataSize; i++) {
            std::cout << data[i];
        }
        std::cout << std::endl;
        std::cout << "------------------" << std::endl;
    }

    std::size_t CharBuffer::getSize() const {
        return dataSize;
    }

    void CharBuffer::write(std::string writeData) {
        changeBufferSizeToAtLeast(dataSize + writeData.size());

        std::memcpy(&(data[dataSize]), writeData.data(), writeData.size());
        dataSize += writeData.size();
        data[dataSize] = 0;
    }

    std::size_t CharBuffer::write(char* writeData, std::size_t writeSize) {
        changeBufferSizeToAtLeast(dataSize + writeSize);
        
        std::memcpy(&(data[dataSize]), writeData, writeSize);
        dataSize += writeSize;
        data[dataSize] = 0;

        return writeSize;
    }

    std::size_t CharBuffer::read(char* buffer, std::size_t bufferSize) {
        std::size_t readSize = (&(data[dataSize]) - position) * sizeof(char);
        if (readSize > bufferSize) {
            readSize = bufferSize;
        }
        std::memcpy(buffer, position, readSize);
        position += readSize / sizeof(char);

        return readSize;
    }

    void CharBuffer::clear() {
        changeBufferSize(0);
    }

    std::span<char> CharBuffer::getSpan() {
        return std::span{data, dataSize};
    }

    std::string CharBuffer::getString() const {
        return std::string{data, dataSize};
    }

    void CharBuffer::changeBufferSize(std::size_t size) {
        char* ptr = static_cast<char*>(std::realloc(data, size + 1));
        if(!ptr) {
            throw Exception("Out of memory. realloc() returned NULL",
                __FILE__, __LINE__);
        }

#       ifdef LIBFILESYNC_CURL_UTILITY_DEBUG_BUILD
            if (ptr != data) {
                monitorReallocation(size + 1);
            }
#       endif

        bufferSize = size;
        data = ptr;
        if (dataSize > bufferSize) {
            dataSize = bufferSize;
            data[dataSize] = 0;
        }
        resetPosition();
    }

    void CharBuffer::changeBufferSizeToAtLeast(std::size_t size) {
        if (size > bufferSize) {
            changeBufferSize(size);
        }
    }

    void CharBuffer::monitorReallocation(std::size_t newSize) {
        reallocations++;
        std::string outputSize;
        if (newSize < 1024) {
            outputSize = std::to_string(newSize) + "B";
        } else if (newSize < 1024 * 1024) {
            outputSize = std::to_string(newSize / 1024) + "KB";
        } else if (newSize < 1024 * 1024 * 1024) {
            outputSize = std::to_string(newSize / 1024 / 1024) + "MB";
        }
        LIBFILESYNC_CURL_UTILITY_DEBUG(
            "Reallocated '" << outputSize << "' of CharBuffer data. "\
            "Number of reallocations: " << reallocations);
    } 

    bool operator==(const CharBuffer& lhs, const CharBuffer& rhs) {
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

    bool operator!=(const CharBuffer& lhs, const CharBuffer& rhs) {
        return !(lhs== rhs);
    }

}