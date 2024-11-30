#include <libfilesync/core/FileSyncer.hpp>

#include <filesystem>

namespace filesync::core {

    FileSyncer::FileSyncer(
        sync_data::Entry& syncContent,
        protocol::Client& protocolClient,
        conflict::Resolver& resolver) :
            FileSyncer{syncContent,
                protocolClient,
                resolver,
                std::make_shared<FileSyncLocks>()} {

    }

    FileSyncer::FileSyncer(
        sync_data::Entry& syncContent,
        protocol::Client& protocolClient,
        conflict::Resolver& resolver,
        std::shared_ptr<FileSyncLocks> locks) :
            protocolClient{protocolClient},
            resolver{resolver},
            locks{std::move(locks)} {

        syncContent.registerObserver(*this);
    }

    bool FileSyncer::fileExistsLocally(sync_data::Entry* entry) {
        return entry->validate();
    }

    bool FileSyncer::fileExistsRemotely(sync_data::Entry* entry) {
        return getProtocolClient().existsOnServer(entry->getRemotePath());
    }

    protocol::Client& FileSyncer::getProtocolClient() {
        return protocolClient;
    }

    conflict::Resolver& FileSyncer::getResolver() {
        return resolver;
    }

    FileSyncLocks& FileSyncer::getLocks() {
        return *locks;
    }

}