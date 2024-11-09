#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <filesystem>

namespace filesync::core {

    FileSyncer::FileSyncer(
        sync_data::Entry& syncContent,
        ProtocolClient& protocolClient,
        conflict::Resolver& resolver) :
            protocolClient{protocolClient},
            resolver{resolver} {

        DEBUG(syncContent.getPath());
        DEBUG(this);
        syncContent.registerObserver(*this);
    }

    bool FileSyncer::fileExistsLocally(sync_data::Entry* entry) {
        return entry->validate();
    }

    bool FileSyncer::fileExistsRemotely(sync_data::Entry* entry) {
        return getProtocolClient().existsOnServer(entry->getRemotePath());
    }

    ProtocolClient& FileSyncer::getProtocolClient() {
        return protocolClient;
    }

    conflict::Resolver& FileSyncer::getResolver() {
        return resolver;
    }

}