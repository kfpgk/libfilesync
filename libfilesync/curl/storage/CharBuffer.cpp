#include <libfilesync/curl/storage/CharBuffer.hpp>
#include <libfilesync/curl/Exception.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace filesync::curl::storage {

    CharBuffer::CharBuffer() :
        data{static_cast<char*>(std::malloc(1))},
        size{0},
        position{0} {
    }

    CharBuffer::CharBuffer(std::string data) :
        data{static_cast<char*>(std::malloc(1))},
        size{0},
        position{0} {

        write(data);
    }

    CharBuffer::CharBuffer(char* data, std::size_t dataSize) :
        data{static_cast<char*>(std::malloc(1))},
        size{0},
        position{0} {

        write(data, dataSize);
    }

    CharBuffer::~CharBuffer() {
        std::free(data);
    }

    void CharBuffer::resetReadPosition() {
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
        char* ptr = static_cast<char*>(std::realloc(data, size + writeData.size() + 1));
        if(!ptr) {
            throw Exception("Out of memory. realloc() returned NULL",
                __FILE__, __LINE__);
        }
        
        data = ptr;
        std::memcpy(&(data[size]), writeData.data(), writeData.size());
        size += writeData.size();
        data[size] = 0;
     
    }

    std::size_t CharBuffer::write(char* writeData, std::size_t dataSize) {
        char* ptr = static_cast<char*>(std::realloc(data, size + dataSize + 1));
        if(!ptr) {
            throw Exception("Out of memory. realloc() returned NULL",
                __FILE__, __LINE__);
        }
        
        data = ptr;
        std::memcpy(&(data[size]), writeData, dataSize);
        size += dataSize;
        data[size] = 0;

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
        char* ptr = static_cast<char*>(std::realloc(data, 1));
        data = ptr;
        size = 0;
        position = 0;
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

    std::span<char> CharBuffer::getSpan() const {
        return std::span{data, size};
    }

    bool operator!=(const CharBuffer& lhs, const CharBuffer& rhs) {
        return !(lhs== rhs);
    }

}