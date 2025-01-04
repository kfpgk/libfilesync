#include <libfilesync/curl/storage/CharBuffer.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/utility/Debug.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace filesync::curl::storage {

    CharBuffer::CharBuffer() :
        owning{true} {

        clear();
    }

    CharBuffer::CharBuffer(std::size_t size) :
        owning{true} {

        changeBufferSize(size);
    }

    CharBuffer::CharBuffer(std::string data) :
        owning{true} {

        clear();
        write(data);
    }

    CharBuffer::CharBuffer(char* data, std::size_t dataSize) :
        owning{true} {

        clear();

        std::size_t remaining = dataSize;
        while (remaining > 0) {
            std::size_t written = write(data, dataSize);
            remaining -= written;
        }
    }

    CharBuffer::CharBuffer(std::span<char>& data) :
        data{data.data()},
        dataSize{data.size()},
        position{this->data},
        owning{false} {

    }

    CharBuffer::~CharBuffer() {
        if (owning) {
            std::free(data);
        }
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

    bool CharBuffer::hasMemoryOwnership() const {
        return owning;
    }

    void CharBuffer::checkOwnership() const {
        if (!owning) {
            throw Exception("Do not own underlying memory. Cannot write to this buffer",
                __FILE__, __LINE__);            
        }
    }

    void CharBuffer::changeBufferSize(std::size_t size) {
        checkOwnership();
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