#ifndef LIBFILESYNC_DATA_EXCEPTION_HPP
#define LIBFILESYNC_DATA_EXCEPTION_HPP

#include <libfilesync/FileSyncException.hpp>

#include <string>

namespace filesync::data {

    class Exception : public FileSyncException {

        public:
            explicit Exception(const std::string& errorMessage);
            Exception(const std::string& errorMessage,
                const char *file,
                int line);

    }; 

}

#endif