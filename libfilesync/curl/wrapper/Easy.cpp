#include <libfilesync/curl/wrapper/Easy.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <curl/curl.h>

namespace filesync::curl::wrapper {

    Easy::Easy() {
        DEBUG("Running curl_easy_init()");
        handle = curl_easy_init();
        if (!handle) {
            throw Exception("curl_easy_init() failed", __FILE__, __LINE__);
        }
        rc = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer.data());
        if (rc != CURLE_OK) {
            throw Exception("curl_easy_setopt(CURLOPT_ERRORBUFFER) failed:", rc, \
                __FILE__, __LINE__);
        }
    }

    Easy::~Easy() {
        DEBUG("Running curl_easy_cleanup()");
        curl_easy_cleanup(handle);
    }

    Easy::Easy(const Easy& rhs) {
        DEBUG("Running curl_easy_duphandle()");
        handle = curl_easy_duphandle(rhs.handle);
    }

    Easy::Easy(Easy&& rhs) noexcept {
        using std::swap;
        swap(*this, rhs);
    }    

    /**
     * @brief Combined copy- and move assignment operator
     * 
     * Copy-and-swap idiom
     */
    Easy& Easy::operator=(Easy rhs) { 
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    void swap(Easy& lhs, Easy& rhs) {
        using std::swap;
        swap(lhs.handle, rhs.handle);        
    }

    void Easy::run() const {
        DEBUG("Running curl_easy_perform()");
        CURLcode rc = curl_easy_perform(handle);
        if (rc != CURLE_OK) {
            throw Exception("curl_easy_perform() failed:", \
                rc, errorBuffer.data(), \
                __FILE__, __LINE__);
        }        
    }

}