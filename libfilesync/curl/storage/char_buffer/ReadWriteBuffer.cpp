#include <libfilesync/curl/storage/char_buffer/ReadWriteBuffer.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/utility/Debug.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>

namespace filesync::curl::storage::char_buffer {

    ReadWriteBuffer::ReadWriteBuffer(std::size_t size) {
        changeBufferSize(size);
    }

    ReadWriteBuffer::ReadWriteBuffer(const std::string& data) {
        write(data);
    }

    ReadWriteBuffer::ReadWriteBuffer(std::span<char> data) :
        ReadWriteBuffer(data.data(), data.size()) {
    }

    ReadWriteBuffer::ReadWriteBuffer(char* data, std::size_t dataSize) {
        std::size_t remaining = dataSize;
        char* dataPtr = data;
        while (remaining > 0) {
            std::size_t written = write(dataPtr, remaining);
            remaining -= written;
            dataPtr += written;
        }
    }

    ReadWriteBuffer::ReadWriteBuffer(const ReadWriteBuffer& rhs) {
        changeBufferSize(rhs.dataSize);
        std::size_t remaining = rhs.dataSize;
        char* dataPtr = rhs.data;
        while (remaining > 0) {
            std::size_t written = write(dataPtr, remaining);
            remaining -= written;
            dataPtr += written;
        }
    }

    ReadWriteBuffer::ReadWriteBuffer(ReadWriteBuffer&& rhs) noexcept {
        using std::swap;
        swap(*this, rhs);
    }

    ReadWriteBuffer& ReadWriteBuffer::operator=(ReadWriteBuffer rhs) {
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    ReadWriteBuffer::~ReadWriteBuffer() {
        std::free(data);
    }

    void swap(ReadWriteBuffer& lhs, ReadWriteBuffer& rhs) noexcept {
        lhs.data = std::exchange(rhs.data, lhs.data);
        lhs.dataSize = std::exchange(rhs.dataSize, lhs.dataSize);
        lhs.bufferSize = std::exchange(rhs.bufferSize, lhs.bufferSize);
        lhs.position = std::exchange(rhs.position, lhs.position);
        lhs.reallocations = std::exchange(rhs.reallocations, lhs.reallocations);
    }

    void ReadWriteBuffer::write(const std::string& writeData) {
        changeBufferSizeToAtLeast(dataSize + writeData.size());

        std::memcpy(&(data[dataSize]), writeData.data(), writeData.size());
        dataSize += writeData.size();
        data[dataSize] = 0;
    }

    std::size_t ReadWriteBuffer::write(char* writeData, std::size_t writeSize) {
        changeBufferSizeToAtLeast(dataSize + writeSize);
        
        std::memcpy(&(data[dataSize]), writeData, writeSize);
        dataSize += writeSize;
        data[dataSize] = 0;

        return writeSize;
    }

    void ReadWriteBuffer::clear() {
        changeBufferSize(0);
    }

    void ReadWriteBuffer::changeBufferSize(std::size_t size) {
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

    void ReadWriteBuffer::changeBufferSizeToAtLeast(std::size_t size) {
        if (size > bufferSize) {
            changeBufferSize(size);
        }
    }

    void ReadWriteBuffer::monitorReallocation(std::size_t newSize) {
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

    bool operator==(const ReadWriteBuffer& lhs, const ReadWriteBuffer& rhs) {
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

    bool operator!=(const ReadWriteBuffer& lhs, const ReadWriteBuffer& rhs) {
        return !(lhs== rhs);
    }

}