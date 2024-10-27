#ifndef LIBFILESYNC_FILE_HPP
#define LIBFILESYNC_FILE_HPP

#include <libfilesync/data/Entry.hpp>

#include <filesystem>

namespace filesync {

    /**
     * Class representing a `file` file system entry which
     * can be synced.
     * 
     * Patterns:
     *  - Leaf object of the Composite pattern
     */
    class File : public Entry {

        public:
            explicit File(const std::filesystem::path& path);

    };

}

#endif