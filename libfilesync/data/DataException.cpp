#include <libfilesync/data/DataException.hpp>

#include <string>

namespace filesync {

    DataException::DataException(const std::string& errorMessage) :
        FileSyncException(errorMessage) {

    }

    DataException::DataException(const std::string& errorMessage,
        const char *file,
        int line) :
            FileSyncException(errorMessage, file, line) {

    }

}