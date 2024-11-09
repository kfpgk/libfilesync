#include <libfilesync/core/BufferedSyncer.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <filesystem>

namespace filesync::core {

    BufferedSyncer::BufferedSyncer(
        sync_data::Entry& syncContent,
        ProtocolClient& protocolClient,
        conflict::Resolver& resolver) :
            FileSyncer{syncContent, protocolClient, resolver} {

    }

    void BufferedSyncer::doUpdate(sync_data::Entry* entry) {
        DEBUG_ENTER();

        if (entry) {
            // if (entry->localChanged()) {
            //     if (entry->remoteChanged()) {
            //         resolver.get().resolve(entry);
            //     } else {
            //         getProtocolClient().upload(entry->getPath(), entry->getRemotePath());
            //     }
            // } else {
            //     if (entry->remoteChanged()) {
            //         overwriteLocal(entry);
            //     } else {
            //         /* do nothing */ 
            //     }                
            // }
            // entry->writeCache();
        }

        DEBUG_EXIT();
    }

}