#include <libfilesync/data/Entry.hpp>

namespace filesync::data {

    Entry::Entry(const std::filesystem::path& path) :
        EntryBase{path} {

    }

}