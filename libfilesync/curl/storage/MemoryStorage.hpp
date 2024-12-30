#ifndef LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_HPP
#define LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_HPP

#include <libfilesync/curl/storage/CharBuffer.hpp>
#include <libfilesync/curl/option/Factory.hpp>

#include <span>

namespace filesync::curl::storage {

   /**
     * @brief Callback function for cURL WRITEFUNCTION option
     * which writes downloaded content to memory.
     * 
     * Data received via cURL is handed to this function
     * for processing (e.g. storing).
     *
     * This function must be callable from C code (libcurl).
     * 
     * This is why we forwared declare it using 'extern "C"'
     * because 'extern "C"' cannot be used inside class. The 
     * function is declared a friend function inside the
     * class definition.
     */
    extern "C" size_t memoryStorageWriteCallback(
        char* contents, size_t size, size_t count, void* target);

    /**
     * @brief Callback function for cURL READFUNCTION option
     * which reads content to be uploaded from memory.
     * 
     * Data received via cURL is handed to this function
     * for processing (e.g. storing).
     *
     * This function must be callable from C code (libcurl).
     * 
     * This is why we forwared declare it using 'extern "C"'
     * because 'extern "C"' cannot be used inside class. The 
     * function is declared a friend function inside the
     * class definition.
     */
    extern "C" size_t memoryStorageReadCallback(
        char* buffer, size_t size, size_t count, void* contents);

    /**
     * @brief Class that stores CURL transfer objects
     * in memory.
     * 
     * Patterns:
     * 
     *  - Element of visitor pattern
     */
    class MemoryStorage {

        public:
            MemoryStorage() = default;
            explicit MemoryStorage(std::span<char> data);

            std::span<char> getDataReference();
            void setupRead(const option::Factory& optionFactory);
            void setupWrite(const option::Factory& optionFactory);
            void flush();
            
        private:
            CharBuffer data;

            friend size_t memoryStorageWriteCallback(
                char* contents, size_t size, size_t count, void* target);  

            friend size_t memoryStorageReadCallback(
                char* buffer, size_t size, size_t count, void* contents);

    };

}

#endif