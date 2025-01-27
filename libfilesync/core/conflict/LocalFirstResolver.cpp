#include <libfilesync/core/conflict/LocalFirstResolver.hpp>

namespace filesync::core::conflict {

    LocalFirstResolver::LocalFirstResolver(
        protocol::Client& protocolClient) :
            Resolver(protocolClient) {

    }

    void LocalFirstResolver::doResolve(sync_data::Entry* entry) {
        getProtocolClient().upload(entry->getPath(), entry->getRemotePath());
    }

}