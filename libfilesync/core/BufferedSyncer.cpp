#include <libfilesync/core/BufferedSyncer.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <filesystem>

namespace filesync::core {

    BufferedSyncer::BufferedSyncer(
        sync_data::Entry& syncContent,
        protocol::Client& protocolClient,
        conflict::Resolver& resolver) :
            FileSyncer{syncContent, protocolClient, resolver} {

    }

    void BufferedSyncer::doUpdate(sync_data::Entry* entry) {
        DEBUG_ENTER();

        if (entry) {

            if (fileExistsLocally(entry)) {
                if (fileExistsRemotely(entry)) {
                    DEBUG("File exists locally and on remote server. Downloading remote to buffer...");
                    getProtocolClient().download( 
                        std::get<std::filesystem::path>(entry->getRemoteBufferLocation()),
                        entry->getRemotePath());
                    if (entry->localDifferentThanPrev()) {
                        DEBUG("Local file changed.");
                        if (entry->remoteDifferentThanPrev()) {
                            DEBUG("Remote file also changed. Need to resolve...");
                            getResolver().resolve(entry);
                        } else {
                            DEBUG("Remote file unchanged. Uploading...");
                            getProtocolClient().upload(entry->getPath(), entry->getRemotePath());
                        }
                    } else {
                        DEBUG("Local file unchanged.");
                        if (entry->remoteDifferentThanPrev()) {
                            DEBUG("Remote file changed."\
                                "Overwrite local file with previously buffered remote file.");
                            entry->writeRemoteBufferToLocal();
                        } else {
                            /* do nothing */ 
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
            entry->setPrevious();
        }

        DEBUG_EXIT();
    }

}