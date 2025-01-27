#include <libfilesync/curl/Exception.hpp>

#include <sstream>

namespace filesync::curl {

    Exception::Exception(const std::string& errorMessage) :
            FileSyncException(errorMessage) {

    }

    Exception::Exception(const std::string& errorMessage,
        const char *file,
        int line) :
            FileSyncException(errorMessage, file, line) {

    }

    Exception::Exception(
        const std::string& errorMessage,
        CURLUcode curlUCode,
        const char *file,
        int line) :
            FileSyncException(errorMessage, file, line),
            curlUCode{curlUCode} {

        std::stringstream message;
        message << errorMessage << std::endl;
        message << getCurlUCodeMessage() << std::endl;
        setErrorMessage(message.str());  
    }

    Exception::Exception(
        const std::string& errorMessage,
        CURLcode curlCode,
        const char *file,
        int line) :
            FileSyncException(errorMessage, file, line),
            curlCode {curlCode} {

        std::stringstream message;
        message << errorMessage << std::endl;
        message << getCurlCodeMessage() << std::endl;
        setErrorMessage(message.str());  
    }

    Exception::Exception(
        const std::string& errorMessage,
        CURLcode curlCode,
        const char* curlErrorBuffer,
        const char *file,
        int line) :
            FileSyncException(errorMessage, file, line),
            curlCode{curlCode},
            curlErrorBuffer{curlErrorBuffer} {

        std::stringstream message;
        message << errorMessage << std::endl;
        message << getCurlCodeMessage() << std::endl;
        message << getCurlErrorBufferMessage() << std::endl;
        setErrorMessage(message.str());  
    }

    CURLcode Exception::getCurlCode() const {
        return curlCode;
    }

    CURLUcode Exception::getCurlUCode() const {
        return curlUCode;
    }

    std::string Exception::getCurlCodeMessage() const {
        std::stringstream message;
        std::string curlCodeErrorString = std::string(curl_easy_strerror(curlCode));
        message << "CURLcode[" << curlCode << "]: '" \
            << curlCodeErrorString << "'";
        return message.str();
    }

    std::string Exception::getCurlUCodeMessage() const {
        std::stringstream message;
        std::string curlUCodeErrorString = std::string(curl_url_strerror(curlUCode));
        message << "CURLUcode[" << curlUCode << "]: '" \
            << curlUCodeErrorString << "'";
        return message.str();
    }

    std::string Exception::getCurlErrorBufferMessage() const {
        std::stringstream message;
        if (!curlErrorBuffer.empty()) {
            message << "CURL ERRORBUFFER: '" << curlErrorBuffer << "'";
        }
        return message.str();
    }

}