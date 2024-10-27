#ifndef LIBFILESYNC_DATA_EXCEPTION_HPP
#define LIBFILESYNC_DATA_EXCEPTION_HPP

#include <libfilesync/FileSyncException.hpp>

#include <string>

namespace filesync {

    class DataException : public FileSyncException {

        public:
            explicit DataException(const std::string& errorMessage);
            DataException(const std::string& errorMessage,
                const char *file,
                int line);

    }; 

}

#endif