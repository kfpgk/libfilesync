#ifndef LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_READ_WRITE_BUFFER_HPP
#define LIBFILESYNC_CURL_STORAGE_CHAR_BUFFER_READ_WRITE_BUFFER_HPP

#include <libfilesync/curl/storage/char_buffer/ReadBuffer.hpp>

#include <cstddef>
#include <span>
#include <string>
#include <vector>

namespace filesync::curl::storage::char_buffer {

    namespace unit_test {
        class ReadWriteTest;
    }

    /**
     * @brief Char buffer for reading and writing
     * from libcurl. 
     * 
     * The buffer has ownership
     * over the underlying memory.
     */
    class ReadWriteBuffer : public ReadBuffer {

        public:
            ReadWriteBuffer() = default;
            explicit ReadWriteBuffer(std::size_t size);
            explicit ReadWriteBuffer(const std::string& data);
            explicit ReadWriteBuffer(std::span<char> data);
            ReadWriteBuffer(char* data, std::size_t dataSize);
            ReadWriteBuffer(const ReadWriteBuffer& rhs);
            ReadWriteBuffer(ReadWriteBuffer&& rhs) noexcept;
            ReadWriteBuffer& operator=(ReadWriteBuffer rhs);
            ~ReadWriteBuffer();
            friend void swap(ReadWriteBuffer& lhs, ReadWriteBuffer& rhs) noexcept;

            void write(const std::string& data);
            [[nodiscard]] std::size_t write(char* data, std::size_t dataSize);
            void clear();

        private:
            std::size_t bufferSize = 0;
            std::size_t reallocations = 0;

            void changeBufferSize(std::size_t size);
            void changeBufferSizeToAtLeast(std::size_t size);
            void monitorReallocation(std::size_t newSize);

        friend bool operator==(const ReadWriteBuffer& lhs, const ReadWriteBuffer& rhs);
        friend bool operator!=(const ReadWriteBuffer& lhs, const ReadWriteBuffer& rhs);

        friend class unit_test::ReadWriteTest;

    };

}

#endif