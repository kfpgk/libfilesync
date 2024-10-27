#include <libfilesync/core/LocalFirstFileSyncer.hpp>
#include <libfilesync/data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>

namespace filesync::core {

    LocalFirstFileSyncer::LocalFirstFileSyncer(
        Entry& syncContent,
        ProtocolClient& protocolClient) :
            FileSyncer(syncContent, protocolClient) {

    }

    void LocalFirstFileSyncer::doResolve(Entry* entry) {
        getProtocolClient().upload(entry->getPath(), entry->getRemotePath());
    }

}