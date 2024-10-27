#include <libfilesync/curl/Exception.hpp>

#include <curl/curl.h>

#include <string>
#include <utility>

namespace filesync::curl::interface {

    /**
     * Sets a CURLoption to the value specified in 'args'
     * via perfect forwarding of 'args' to the libcurl API
     */
    template<typename... Args> 
    void Easy::setOption(CURLoption option, Args&&... args) {
        CURLcode rc = curl_easy_setopt(handle, option, std::forward<Args>(args)...);
        if (rc != CURLE_OK) {
            throw Exception(::std::string("curl_easy_setopt(" \
                + std::string(curl_easy_option_by_id(option)->name) \
                + ") failed:"), rc, errorBuffer.data(), \
                __FILE__, __LINE__);
        }
    }

}