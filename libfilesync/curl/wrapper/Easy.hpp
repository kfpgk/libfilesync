#ifndef LIBFILESYNC_CURL_WRAPPER_EASY_HPP
#define LIBFILESYNC_CURL_WRAPPER_EASY_HPP

#include <libfilesync/curl/wrapper/Base.hpp>
#include <libfilesync/curl/Exception.hpp>

#include <curl/curl.h>

#include <array>

namespace filesync::curl::wrapper {

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