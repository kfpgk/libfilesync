#include <libfilesync/core/conflict/RemoteFirstResolver.hpp>

namespace filesync::core::conflict {

    RemoteFirstResolver::RemoteFirstResolver(
        ProtocolClient& protocolClient) :
            Resolver(protocolClient) {

    }

    void RemoteFirstResolver::doResolve(sync_data::Entry* entry) {
        getProtocolClient().download(entry->getPath(), entry->getRemotePath());
    }

}