#include <libfilesync/core/conflict/Resolver.hpp>

namespace filesync::core::conflict {

    Resolver::Resolver(ProtocolClient& protocolClient) :
        protocolClient{protocolClient} {

    }

    ProtocolClient& Resolver::getProtocolClient() {
        return protocolClient;
    }

    void Resolver::resolve(sync_data::Entry* entry) {
        doResolve(entry);
    }

}