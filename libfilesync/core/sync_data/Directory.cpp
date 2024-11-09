#include <libfilesync/core/sync_data/Directory.hpp>

namespace filesync::core::sync_data {

    Directory::Directory(const std::filesystem::path& path) :
        DirectoryBase{path},
        Entry{path},
        EntryBase{path} {

    }

    void Directory::doPrint() const {
        Entry::doPrint();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->print();        
        }        
    }

    void Directory::doSetRemoteEntry(const std::filesystem::path& path) {
        Entry::doSetRemoteEntry(path);
        for(auto it = components.begin(); it != components.end(); it++) {
            namespace fs = std::filesystem;
            fs::path childPath = it->get()->getPath();
            fs::path childRemote = path.string() + fs::path::preferred_separator
                + childPath.filename().string();
            childRemote.make_preferred();
            it->get()->setRemoteEntry(childRemote.string());
        }
    }

}