#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_READ_BUFFER_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_READ_BUFFER_HPP

#include <cstddef>
#include <span>
#include <string>

namespace filesync::curl::storage::char_buffer {

    namespace unit_test {
        class ReadBufferTest;
    }

    /**
     * @brief Char read only buffer for uploading to 
     * libcurl. 
     * 
     * The buffer does not have ownership over the
     * underlying memory.
     */
    class ReadBuffer {

        public:
            ReadBuffer(char* data, std::size_t dataSize);
            explicit ReadBuffer(std::string& data);
            explicit ReadBuffer(std::span<char> data);
            explicit ReadBuffer(ReadBuffer& data);
            ReadBuffer(const ReadBuffer& rhs);
            ReadBuffer(ReadBuffer&& rhs) noexcept;
            ReadBuffer& operator=(ReadBuffer rhs);
            ~ReadBuffer() = default;
            friend void swap(ReadBuffer& lhs, ReadBuffer& rhs) noexcept;

            void resetPosition();
            void print();
            [[nodiscard]] std::size_t getSize() const;
            [[nodiscard]] std::size_t read(char* buffer, std::size_t bufferSize);
            std::span<char> getSpan();
            std::string getString() const;

        protected:
            char* data = nullptr;
            std::size_t dataSize = 0;
            char* position = nullptr;

            ReadBuffer() = default;

        friend bool operator==(
            const ReadBuffer& lhs, const ReadBuffer& rhs);
        friend bool operator!=(
            const ReadBuffer& lhs, const ReadBuffer& rhs);

        friend class unit_test::ReadBufferTest;

    };

}

#endif