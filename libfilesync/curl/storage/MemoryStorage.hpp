#ifndef LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_HPP
#define LIBFILESYNC_CURL_STORAGE_MEMORY_STORAGE_HPP

#include <libfilesync/curl/storage/Storage.hpp>
#include <libfilesync/curl/option/Factory.hpp>

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
    extern "C" size_t writeToMemory(char *contents, size_t size, size_t count, void *target);

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
    extern "C" size_t readFromMemory(char *buffer, size_t size, size_t count, void *contents);

    class MemoryStorage : public Storage {

        private:
            void doSetupRead(const option::Factory& optionFactory) override;
            void doSetupWrite(const option::Factory& optionFactory) override;
            void doFlush() override;

            friend size_t writeToMemory(
                char *contents, size_t size, size_t count, void* target);  

            friend size_t readFromMemory(
                char *buffer, size_t size, size_t count, void* contents);


    };

}

#endif