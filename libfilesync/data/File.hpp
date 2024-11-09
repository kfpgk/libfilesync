#ifndef LIBFILESYNC_DATA_FILE_HPP
#define LIBFILESYNC_DATA_FILE_HPP

#include <libfilesync/data/Entry.hpp>

#include <filesystem>

namespace filesync::data {

    /**
     * Class representing a `file` file system entry which
     * can be synced.
     * 
     * Patterns:
     *  - Leaf object of the Composite pattern
     */
    template<typename T>
    class FileBase : virtual public EntryBase<T> {

        protected:
            explicit FileBase(const std::filesystem::path& path);

    };

    class File : public Entry, public FileBase<Entry> {

        public:
            explicit File(const std::filesystem::path& path); 

    };

}

#include <libfilesync/data/File.tpp>

#endif