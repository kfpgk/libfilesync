#ifndef LIBFILESYNC_CURL_WRAPPER_EASY_HPP
#define LIBFILESYNC_CURL_WRAPPER_EASY_HPP

#include <libfilesync/curl/wrapper/Base.hpp>
#include <libfilesync/curl/Exception.hpp>

#include <curl/curl.h>

#include <array>
#include <cstdarg>
#include <cstdio>
#include <string>

namespace filesync::curl::wrapper {

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
    extern "C" size_t easyDefaultWriteCallback(char *contents, size_t size, size_t count, FILE *target);

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
    extern "C" size_t easyDefaultReadCallback(char *buffer, size_t size, size_t count, FILE *contents);

    /**
     * @brief Wrapper for the cURL easy interface
     * 
     * Performs global libcurl initialization and cleanup
     * if required.
     */
    class Easy : public Base {

        public:
            Easy();
            virtual ~Easy();
            Easy(const Easy& rhs);
            Easy(Easy&& rhs) noexcept;
            Easy& operator=(Easy rhs);
            friend void swap(Easy& lhs, Easy& rhs);

            friend size_t easyDefaultWriteCallback(
                char *contents, size_t size, size_t count, FILE *target);  

            friend size_t easyDefaultReadCallback(
                char *buffer, size_t size, size_t count, FILE *contents);

            template<typename... Args> 
            void setOption(CURLoption option, Args&&... args);

            virtual void run() const;

        private:
            CURL* handle;
            CURLcode rc;
            std::array<char, CURL_ERROR_SIZE> errorBuffer;
            
    };

}

#include <libfilesync/curl/wrapper/Easy.tpp>

#endif