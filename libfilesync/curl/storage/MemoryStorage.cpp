#include <libfilesync/curl/storage/MemoryStorage.hpp>
#include <libfilesync/curl/storage/char_buffer/visitor/GetSpan.hpp>
#include <libfilesync/curl/storage/char_buffer/visitor/ResetPosition.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/utility/Debug.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>

namespace filesync::curl::storage {

    MemoryStorage::MemoryStorage(std::size_t bufferSize) :
        data{char_buffer::ReadWriteBuffer(bufferSize)},
        initialBufferSize{bufferSize} {

    }

    MemoryStorage::MemoryStorage(std::span<char> data) :
        data{char_buffer::ReadBuffer(data)} {

    }

    MemoryStorage::MemoryStorage(
        const char_buffer::unit_test::CharBufferStub& charBufferStub) :
            data{charBufferStub} {

    }

    MemoryStorage::MemoryStorage(const MemoryStorage& rhs) {
        data = rhs.data;
        initialBufferSize = initialBufferSize;
    }

    MemoryStorage::MemoryStorage(MemoryStorage&& rhs) {
        using std::swap;
        swap(*this, rhs);
    }

    MemoryStorage& MemoryStorage::operator=(MemoryStorage rhs) {
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    void swap(MemoryStorage& lhs, MemoryStorage& rhs) {
        using std::swap;
        swap(lhs.data, rhs.data);
        lhs.initialBufferSize = std::exchange(rhs.initialBufferSize, lhs.initialBufferSize);
    }

    std::span<char> MemoryStorage::getDataReference() {
        return std::visit(char_buffer::visitor::GetSpan{}, data);
    }

    void MemoryStorage::setupRead(const option::Factory& optionFactory) {
        optionFactory.createGeneric(CURLOPT_READFUNCTION, &memoryStorageReadCallback)->set();
        std::visit(char_buffer::visitor::ResetPosition{}, data);
        optionFactory.createGeneric(CURLOPT_READDATA, static_cast<void*>(&data))->set();
    }

    void MemoryStorage::setupWrite(const option::Factory& optionFactory) {
        optionFactory.createGeneric(CURLOPT_WRITEFUNCTION, &memoryStorageWriteCallback)->set();
        if (!std::holds_alternative<char_buffer::ReadWriteBuffer>(data)) {
            data = char_buffer::ReadWriteBuffer{initialBufferSize};
        }
        std::visit(char_buffer::visitor::ResetPosition{}, data);
        LIBFILESYNC_CURL_UTILITY_DEBUG("Data address: " << &data);
        optionFactory.createGeneric(CURLOPT_WRITEDATA, static_cast<void*>(&data))->set();
    }

    void MemoryStorage::flush() {
        /* Do nothing */
    }

    /**
     * @brief Write callback function which writes incoming
     * data to a memory chunk.
     * 
     * This implementation is based on the `getinmemory.c` 
     * example from the libcurl documentation examples section.
     * 
     * @param[in] target Target memory which the recieved data
     * is written to. If it is NULL, we discard the received
     * data.
     */
    extern "C" size_t memoryStorageWriteCallback(char* contents,
        size_t size, size_t count, void* target) {

        LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER();
        size_t ret = 0;

        if (target) {
            std::size_t dataSize = size * count;
            struct char_buffer::ReadWriteBuffer* data = static_cast<char_buffer::ReadWriteBuffer*>(target);
            
            ret = data->write(contents, dataSize);
            
        } else {
            ret = count;
        }

        LIBFILESYNC_CURL_UTILITY_DEBUG("Return: " << ret);
        LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT();
        return ret;
    }

    /**
     * @brief Read callback function which reads outgoing
     * data from memory.
     * 
     * @param[in] contents Memory which the outgoing data
     * is read from.
     */
    extern "C" size_t memoryStorageReadCallback(char* buffer,
        size_t size, size_t count, void* contents) {

        LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER();
        size_t ret = 0;

        if (contents && buffer) {
            struct char_buffer::ReadWriteBuffer* data = static_cast<char_buffer::ReadWriteBuffer*>(contents);

            std::size_t bufferSize = size * count;

            ret = data->read(buffer, bufferSize);
        } else {
            ret = 0;
        }

        LIBFILESYNC_CURL_UTILITY_DEBUG("Return: " << ret);
        LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT();
        return ret;
    }

}