#include <libfilesync/curl/wrapper/Easy.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/utility/Debug.hpp>

#include <curl/curl.h>

namespace filesync::curl::wrapper {

    Easy::Easy() {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Running curl_easy_init()");
        handle = curl_easy_init();
        if (!handle) {
            throw Exception("curl_easy_init() failed", __FILE__, __LINE__);
        }
        rc = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer.data());
        if (rc != CURLE_OK) {
            throw Exception("curl_easy_setopt(CURLOPT_ERRORBUFFER) failed:", rc, \
                __FILE__, __LINE__);
        }
        setOption(CURLOPT_WRITEFUNCTION, &easyDefaultWriteCallback);
        setOption(CURLOPT_READFUNCTION, &easyDefaultReadCallback);
    }

    Easy::~Easy() {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Running curl_easy_cleanup()");
        curl_easy_cleanup(handle);
    }

    Easy::Easy(const Easy& rhs) {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Running curl_easy_duphandle()");
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

    void Easy::perform() const {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Running curl_easy_perform()");
        CURLcode rc = curl_easy_perform(handle);
        if (rc != CURLE_OK) {
            throw Exception("curl_easy_perform() failed:", \
                rc, errorBuffer.data(), \
                __FILE__, __LINE__);
        }        
    }

    /**
     * @brief Default write callback function which writes incoming
     * data to a file.
     * 
     * Clients of this class may change behavior via setting their
     * own callback via the CURLOPT_WRITEFUNCTION option.
     * 
     * @param[in] target Target file to which the recieved data
     * is written to. If it is NULL, we discard the received
     * data.
     */
    extern "C" size_t easyDefaultWriteCallback(char *contents,
        size_t size, size_t count, FILE *target) {
        if (target) {
            return std::fwrite(contents, size, count, target);
        } else {
            return count;
        }
    }

    /**
     * @brief Default read callback function which reads outgoing
     * data from a file.
     * 
     * Clients of this class may change behavior via setting their
     * own callback via the CURLOPT_READFUNCTION option.
     * 
     * @param[in] contents Source file which the outgoing data
     * is read from.
     */
    extern "C" size_t easyDefaultReadCallback(char *buffer,
        size_t size, size_t count, FILE *contents) {
        return std::fread(buffer, size, count, contents);  
    }

}