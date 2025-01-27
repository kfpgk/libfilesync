#include <libfilesync/core/sync_data/RemoteEntry.hpp>

namespace filesync::core::sync_data {

    RemoteEntry::RemoteEntry(std::string path) :
        path{path} {

            this->path = this->path.relative_path();
    }

    std::string RemoteEntry::getPath() const {
        return path.string();
    }

    std::string RemoteEntry::getLocation() const {
        return path.parent_path();
    }

    std::string RemoteEntry::getFileName() const {
        return path.filename();
    }

}