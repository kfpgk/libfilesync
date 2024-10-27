#include <libfilesync/data/File.hpp>

namespace filesync {

    File::File(const std::filesystem::path& path) :
        Entry(path) {

    }

}