#include <libfilesync/curl/Url.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <cstring>

namespace filesync::curl {

    Url::Url() {
        handle = curl_url();
    }

    Url::~Url() {
        curl_url_cleanup(handle);
        if (urlPointer) {
            curl_free(urlPointer);
        }
    }

    Url::Url(const Url& rhs) {
        handle = curl_url_dup(rhs.handle);
        if (rhs.urlPointer) {
            urlPointer = new char[strlen(rhs.urlPointer) + 1];
            strcpy(urlPointer, rhs.urlPointer);
        }
    }

    Url::Url(Url&& rhs) noexcept {
        using std::swap;
        swap(*this, rhs);
    }

    /**
     * @brief Combined copy- and move assignment operator
     * 
     * Copy-and-swap idiom
     */
    Url& Url::operator=(Url rhs) {
        using std::swap;
        swap(*this, rhs);
        return *this;
    }

    void swap(Url& lhs, Url& rhs) {
        using std::swap;
        swap(lhs.handle, rhs.handle);
        swap(lhs.urlPointer, rhs.urlPointer);        
    }

    char* Url::getUrl() {
        if (urlPointer) {
            curl_free(urlPointer);
            urlPointer = nullptr;            
        }
        CURLUcode rc = curl_url_get(handle, CURLUPART_URL, &urlPointer, 0);
        if (rc != CURLUE_OK || !urlPointer) {
            urlPointer = nullptr;
            throw Exception("curl_url_get(CURLUPART_URL) failed", rc, __FILE__, __LINE__);
        }
        return urlPointer; 
    }

    char* Url::getHost() {
        if (urlPointer) {
            curl_free(urlPointer);
            urlPointer = nullptr;            
        }
        CURLUcode rc = curl_url_get(handle, CURLUPART_HOST, &urlPointer, 0);
        if (rc != CURLUE_OK || !urlPointer) {
            urlPointer = nullptr;
            throw Exception("curl_url_get(CURLUPART_URL) failed", rc, __FILE__, __LINE__);
        }
        return urlPointer; 
    }

    char* Url::getScheme() {
        if (urlPointer) {
            curl_free(urlPointer);
            urlPointer = nullptr;            
        }
        CURLUcode rc = curl_url_get(handle, CURLUPART_SCHEME, &urlPointer, 0);
        if (rc != CURLUE_OK || !urlPointer) {
            urlPointer = nullptr;
            throw Exception("curl_url_get(CURLUPART_URL) failed", rc, __FILE__, __LINE__);
        }
        return urlPointer; 
    }

    char* Url::getPath() {
        if (urlPointer) {
            curl_free(urlPointer);
            urlPointer = nullptr;            
        }
        CURLUcode rc = curl_url_get(handle, CURLUPART_PATH, &urlPointer, 0);
        if (rc != CURLUE_OK || !urlPointer) {
            urlPointer = nullptr;
            throw Exception("curl_url_get(CURLUPART_URL) failed", rc, __FILE__, __LINE__);
        }
        return urlPointer; 
    }

    void Url::setHost(const std::string& host) {
        setPart(CURLUPART_HOST, host.c_str(), 0);
    }

    void Url::setScheme(const std::string& scheme) {
        setPart(CURLUPART_SCHEME, scheme.c_str(), 0);
    }

    void Url::setPath(const std::string& path) {
        setPart(CURLUPART_PATH, path.c_str(), 0);
    }

    void Url::validate() {
        char* localUrlPointer;
        CURLUcode rc = curl_url_get(handle, CURLUPART_URL, &localUrlPointer, 0);
        if (rc != CURLUE_OK || !localUrlPointer) {
            throw Exception("curl_url_get(CURLUPART_URL) failed", rc, __FILE__, __LINE__);
        }
        rc = curl_url_set(handle, CURLUPART_URL, localUrlPointer, 0);
        curl_free(localUrlPointer);
        if (rc != CURLUE_OK) {           
            throw Exception("curl_url_set(CURLUPART_URL) failed. Invalid url: '"\
            + std::string(localUrlPointer) + "'",\
            rc, __FILE__, __LINE__);
        }
    }

}