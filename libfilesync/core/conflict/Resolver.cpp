#include <libfilesync/core/conflict/Resolver.hpp>

namespace filesync::core::conflict {

    Resolver::Resolver(protocol::Client& protocolClient) :
        Resolver{protocolClient, std::make_shared<FileSyncLocks>()} {

    }

    Resolver::Resolver(protocol::Client& protocolClient,
        std::shared_ptr<FileSyncLocks> locks) :
            protocolClient{protocolClient},
            locks{std::move(locks)} {

    }

    protocol::Client& Resolver::getProtocolClient() {
        return protocolClient;
    }

    FileSyncLocks& Resolver::getLocks() {
        return *locks;
    }

    void Resolver::resolve(sync_data::Entry* entry) {
        doResolve(entry);
    }

}