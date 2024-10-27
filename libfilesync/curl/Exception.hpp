#ifndef LIBFILESYNC_CURL_EXCEPTION_HPP
#define LIBFILESYNC_CURL_EXCEPTION_HPP

#include <libfilesync/FileSyncException.hpp>

#include <curl/curl.h>

#include <string>
#include <optional>

namespace filesync::curl {

    class Exception : public FileSyncException {

        public:
            Exception(const std::string& errorMessage);
            Exception(const std::string& errorMessage,
                const char *file,
                int line);
            Exception(const std::string& errorMessage,
                CURLUcode curlCode,
                const char *file,
                int line);
            Exception(const std::string& errorMessage,
                CURLcode curlCode,
                const char *file,
                int line);
            Exception(const std::string& errorMessage,
                CURLcode curlCode,
                const char* curlErrorBuffer,
                const char *file,
                int line);

            CURLcode getCurlCode() const;
            CURLUcode getCurlUCode() const;

        private:
            std::string curlErrorBuffer;
            CURLcode curlCode = CURLE_OK;
            CURLUcode curlUCode = CURLUE_OK;
            std::string getCurlCodeMessage() const;
            std::string getCurlUCodeMessage() const;
            std::string getCurlErrorBufferMessage() const;

    };

}

#endif