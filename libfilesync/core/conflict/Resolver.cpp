#include <libfilesync/core/conflict/Resolver.hpp>

namespace filesync::core::conflict {

    Resolver::Resolver(ProtocolClient& protocolClient) :
        Resolver{protocolClient, std::make_shared<FileSyncLocks>()} {

    }

    Resolver::Resolver(ProtocolClient& protocolClient,
        std::shared_ptr<FileSyncLocks> locks) :
            protocolClient{protocolClient},
            locks{std::move(locks)} {

    }

    ProtocolClient& Resolver::getProtocolClient() {
        return protocolClient;
    }

    FileSyncLocks& Resolver::getLocks() {
        return *locks;
    }

    void Resolver::resolve(sync_data::Entry* entry) {
        doResolve(entry);
    }

}