#include <libfilesync/data/File.hpp>

namespace filesync::data {

    File::File(const std::filesystem::path& path) :
        FileBase{path},
        Entry{path},
        EntryBase{path} {

    }

}