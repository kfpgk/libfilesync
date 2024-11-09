#include <libfilesync/data/Directory.hpp>

namespace filesync::data {

    Directory::Directory(const std::filesystem::path& path) :
        DirectoryBase{path},
        Entry{path},
        EntryBase{path} {

    }

}