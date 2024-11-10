#include <libfilesync/core/UnbufferedSyncer.hpp>
#include <libfilesync/utility/Debug.hpp>

namespace filesync::core {

    UnbufferedSyncer::UnbufferedSyncer(
        sync_data::Entry& syncContent,
        ProtocolClient& protocolClient,
        conflict::Resolver& resolver) :
            FileSyncer{syncContent, protocolClient, resolver} {

    }

    void UnbufferedSyncer::doUpdate(sync_data::Entry* entry) {
        DEBUG_ENTER();

        if (entry) {
            if (fileExistsLocally(entry)) {
                if (fileExistsRemotely(entry)) {
                    if (entry->hasChanged()) {
                        DEBUG("Local file changed. Uploading...");
                        getProtocolClient().upload(entry->getPath(), entry->getRemotePath());
                    } else {
                        if (entry->getSyncInProgress()) {
                            DEBUG("Local file unchanged. Downloading...");
                            getProtocolClient().download(entry->getPath(), entry->getRemotePath()); 
                        } else {
                            DEBUG("Initial sync. Local and remote file exist. Resolve...");
                            getResolver().resolve(entry);
                        }                       
                    }
                } else {
                    DEBUG("Local file not present at remote. Uploading...");
                    getProtocolClient().upload(entry->getPath(), entry->getRemotePath()); 
                }
            } else {
                if (fileExistsRemotely(entry)) {
                    DEBUG("Remote file not present locally. Downloading...");
                    getProtocolClient().download(entry->getPath(), entry->getRemotePath()); 
                } else {
                    DEBUG("Neither local nor remote file present. Do nothing...");
                    /* Do nothing */                       
                }                  
            }
            entry->setSyncInProgress();;
            entry->resetChanged();
        }

        DEBUG_EXIT();
    }

}