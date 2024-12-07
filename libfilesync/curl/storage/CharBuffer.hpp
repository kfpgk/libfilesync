#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_HPP

#include <libfilesync/curl/storage/ConsumingAllocator.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace filesync::curl::storage {

    /**
     * @brief Char buffer for reading and writing
     * from libcurl
     * 
     * This is a low level character buffer. It can
     * be used as a download destination or upload
     * source for CURL transfers.
     */
    class CharBuffer {

        public:
            CharBuffer();
            ~CharBuffer();

            void resetReadPosition();
            void print();
            std::size_t getSize() const;
            void write(std::string data);
            std::size_t write(char* data, std::size_t dataSize);
            std::size_t read(char* buffer, std::size_t bufferSize);
            void clear();
            std::vector<char, ConsumingAllocator<char>> retrieveVector();

        private:
            char* data;
            std::size_t size;
            char* position;

        friend bool operator==(const CharBuffer& lhs, const CharBuffer& rhs);
        friend bool operator!=(const CharBuffer& lhs, const CharBuffer& rhs);

    };

}

#endif