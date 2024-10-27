#include <libfilesync/core/RemoteFirstFileSyncer.hpp>
#include <libfilesync/data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>

namespace filesync::core {

    RemoteFirstFileSyncer::RemoteFirstFileSyncer(
        Entry& syncContent,
        ProtocolClient& protocolClient) :
            FileSyncer(syncContent, protocolClient) {

    }

    void RemoteFirstFileSyncer::doResolve(Entry* entry) {
        getProtocolClient().download(entry->getPath(), entry->getRemotePath());
    }

}