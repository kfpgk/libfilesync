#include <libfilesync/curl/Exception.hpp>

namespace filesync::curl::wrapper {

    template<typename... Args>
    void Url::setPart(CURLUPart part, Args&&... args) {
        CURLUcode rc = curl_url_set(handle, part, std::forward<Args>(args)...);
        if (rc != CURLUE_OK) {
            throw Exception("curl_url_set() failed", rc, __FILE__, __LINE__);
        }
    }

}