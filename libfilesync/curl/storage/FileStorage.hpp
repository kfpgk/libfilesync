#ifndef LIBFILESYNC_CURL_STORAGE_FILE_STORAGE_HPP
#define LIBFILESYNC_CURL_STORAGE_FILE_STORAGE_HPP

#include <libfilesync/curl/option/Factory.hpp>

#include <cstdarg>
#include <cstdio>
#include <filesystem>

namespace filesync::curl::storage {

    namespace unit_test {
        class FileStorageTest;
    }

    /**
     * @brief Callback function for cURL WRITEFUNCTION option
     * which writes downloaded content to a file.
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
    extern "C" size_t fileStorageWriteCallback(
        char* contents, size_t size, size_t count, FILE* target);

    /**
     * @brief Callback function for cURL READFUNCTION option
     * which reads content to be uploaded from a file.
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
    extern "C" size_t fileStorageReadCallback(
        char* buffer, size_t size, size_t count, FILE* contents);

    /**
     * @brief Class that stores CURL transfer objects
     * in a file.
     * 
     * Patterns:
     * 
     *  - Element of visitor pattern
     */
    class FileStorage {
        enum class FileAccessType {read, write}; 

        public:
            explicit FileStorage(const std::filesystem::path& path);
            FileStorage(const FileStorage& rhs);
            FileStorage(FileStorage&& rhs);
            FileStorage& operator=(FileStorage rhs);
            ~FileStorage();
            friend void swap(FileStorage& lhs, FileStorage& rhs);

            std::filesystem::path getPath() const;
            void setupRead(const option::Factory& optionFactory);
            void setupWrite(const option::Factory& optionFactory);
            void flush();

        private:
            std::filesystem::path path;
            std::FILE* filePointer = nullptr;

            [[nodiscard]]  std::FILE* getFilePointer(bool throwIfNull = true) const;

            void setFilePointer(const std::filesystem::path& path,
                FileAccessType fileAccess);
            void setInputFileSizeFromFile(const option::Factory& optionFactory,
                const std::filesystem::path& path);
            void setInputFileSize(const option::Factory& optionFactory,
                std::uintmax_t size);
            void setFile(const option::Factory& optionFactory,
                FileAccessType fileAccess);

        friend size_t fileStorageWriteCallback(
            char *contents, size_t size, size_t count, FILE* target);  

        friend size_t fileStorageReadCallback(
            char *buffer, size_t size, size_t count, FILE* contents);

        friend class unit_test::FileStorageTest;

    };

}

#endif