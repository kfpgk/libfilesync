#include <libfilesync/data/Exception.hpp>

#include <string>

namespace filesync::data {

    Exception::Exception(const std::string& errorMessage) :
        FileSyncException(errorMessage) {

    }

    Exception::Exception(const std::string& errorMessage,
        const char *file,
        int line) :
            FileSyncException(errorMessage, file, line) {

    }

}