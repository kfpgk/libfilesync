#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/utility/Logger.hpp>

using namespace filesync::utility;

namespace filesync::core::sync_data {

    Entry::Entry(const std::filesystem::path& path) :
        EntryBase{path} {

    }

    std::filesystem::path Entry::getRemotePath() const {
        return remoteEntry->getPath();
    }

    void Entry::setRemoteEntry(const std::filesystem::path& path) {
        doSetRemoteEntry(path);
    }

    void Entry::doSetRemoteEntry(const std::filesystem::path& path) {
        remoteEntry = std::make_unique<RemoteEntry>(path);
    }

    void Entry::doPrint() const {
        std::string remotePath = "undefined";
        if (remoteEntry) {
            remotePath = remoteEntry->getPath();
        }
        Logger::getInstance().log(LogDomain::Info,
            std::string(getPath().string() + " <-> <server>/<rootPath>/" + remotePath));        
    }

}