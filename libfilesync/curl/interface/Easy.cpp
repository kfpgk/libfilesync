#include <libfilesync/curl/interface/Easy.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <curl/curl.h>

#include <cstdarg>
#include <cstdio>

namespace filesync::curl::interface {

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
        setOption(CURLOPT_WRITEFUNCTION, &writeToFile);
        setOption(CURLOPT_READFUNCTION, &readFromFile);
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

    /**
     * @brief Write callback function which writes incoming
     * data to a file.
     * 
     * @param[in] target Target file to which the recieved data
     * is written to. If it is NULL, we discard the received
     * data.
     */
    extern "C" size_t writeToFile(char *contents,
        size_t size, size_t count, FILE *target) {
        if (target) {
            return std::fwrite(contents, size, count, target);
        } else {
            return count;
        }
    }

    extern "C" size_t readFromFile(char *buffer,
        size_t size, size_t count, FILE *contents) {
        return std::fread(buffer, size, count, contents);  
    }

}