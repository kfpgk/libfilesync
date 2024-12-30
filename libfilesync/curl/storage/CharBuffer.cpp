#include <libfilesync/curl/storage/CharBuffer.hpp>
#include <libfilesync/curl/Exception.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace filesync::curl::storage {

    CharBuffer::CharBuffer() :
        size{0},
        position{0},
        owning{true} {

        char* ptr = static_cast<char*>(std::malloc(1));
        if(!ptr) {
            throw Exception("Out of memory. malloc() returned NULL",
                __FILE__, __LINE__);
        }
        data = ptr;
    }

    CharBuffer::CharBuffer(std::string data) :
        size{0},
        owning{true} {

        char* ptr = static_cast<char*>(std::malloc(1));
        if(!ptr) {
            throw Exception("Out of memory. malloc() returned NULL",
                __FILE__, __LINE__);
        }
        this->data = ptr;
        position = this->data;

        write(data);
    }

    CharBuffer::CharBuffer(char* data, std::size_t dataSize) :
        size{0},
        owning{true} {

        char* ptr = static_cast<char*>(std::malloc(1));
        if(!ptr) {
            throw Exception("Out of memory. malloc() returned NULL",
                __FILE__, __LINE__);
        }
        this->data = ptr;
        position = this->data;

        std::size_t remaining = dataSize;
        while (remaining > 0) {
            std::size_t written = write(data, dataSize);
            remaining -= written;
        }
    }

    CharBuffer::CharBuffer(std::span<char>& data) :
        data{data.data()},
        size{data.size()},
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
        std::cout << "Size: " << size << std::endl;
        std::cout << "Data:" << std::endl;
        for(int i = 0; i < size; i++) {
            std::cout << data[i];
        }
        std::cout << std::endl;
        std::cout << "------------------" << std::endl;
    }

    std::size_t CharBuffer::getSize() const {
        return size;
    }

    void CharBuffer::write(std::string writeData) {
        checkOwnership();
        char* ptr = static_cast<char*>(std::realloc(data, size + writeData.size() + 1));
        if(!ptr) {
            throw Exception("Out of memory. realloc() returned NULL",
                __FILE__, __LINE__);
        }
        
        data = ptr;
        std::memcpy(&(data[size]), writeData.data(), writeData.size());
        size += writeData.size();
        data[size] = 0;
        resetPosition();
    }

    std::size_t CharBuffer::write(char* writeData, std::size_t dataSize) {
        checkOwnership();
        char* ptr = static_cast<char*>(std::realloc(data, size + dataSize + 1));
        if(!ptr) {
            throw Exception("Out of memory. realloc() returned NULL",
                __FILE__, __LINE__);
        }
        
        data = ptr;
        std::memcpy(&(data[size]), writeData, dataSize);
        size += dataSize;
        data[size] = 0;
        resetPosition();

        return dataSize;
    }

    std::size_t CharBuffer::read(char* buffer, std::size_t bufferSize) {
        std::size_t readSize = (&(data[size]) - position) * sizeof(char);
        if (readSize > bufferSize) {
            readSize = bufferSize;
        }
        std::memcpy(buffer, position, readSize);
        position += readSize / sizeof(char);

        return readSize;
    }

    void CharBuffer::clear() {
        checkOwnership();
        char* ptr = static_cast<char*>(std::realloc(data, 1));
        if(!ptr) {
            throw Exception("Out of memory. realloc() returned NULL",
                __FILE__, __LINE__);
        }
        data = ptr;
        size = 0;
        resetPosition();
    }

    std::span<char> CharBuffer::getSpan() {
        return std::span{data, size};
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

    bool operator==(const CharBuffer& lhs, const CharBuffer& rhs) {
        if (lhs.size != rhs.size) {
            return false;
        }
        for (int i; i < lhs.size; i++) {
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