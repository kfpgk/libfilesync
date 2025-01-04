#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_HPP

#include <cstddef>
#include <span>
#include <string>
#include <vector>

namespace filesync::curl::storage {

    namespace unit_test {
        class CharBufferTest;
    }

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
            explicit CharBuffer(std::size_t size);
            explicit CharBuffer(std::string data);
            CharBuffer(char* data, std::size_t dataSize);
            CharBuffer(std::span<char>& data);
            ~CharBuffer();

            void resetPosition();
            void print();
            [[nodiscard]] std::size_t getSize() const;
            void write(std::string data);
            [[nodiscard]] std::size_t write(char* data, std::size_t dataSize);
            [[nodiscard]] std::size_t read(char* buffer, std::size_t bufferSize);
            void clear();
            std::span<char> getSpan();
            bool hasMemoryOwnership() const;

        private:
            char* data = nullptr;
            std::size_t dataSize = 0;
            std::size_t bufferSize = 0;
            char* position = nullptr;
            bool owning = false;
            std::size_t reallocations = 0;

            void checkOwnership() const;
            void changeBufferSize(std::size_t size);
            void changeBufferSizeToAtLeast(std::size_t size);
            void monitorReallocation(std::size_t newSize);

        friend bool operator==(const CharBuffer& lhs, const CharBuffer& rhs);
        friend bool operator!=(const CharBuffer& lhs, const CharBuffer& rhs);

        friend class unit_test::CharBufferTest;

    };

}

#endif