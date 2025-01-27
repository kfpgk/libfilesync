#include <libfilesync/core/OneWaySyncer.hpp>
#include <libfilesync/utility/Debug.hpp>

namespace filesync::core {

    OneWaySyncer::OneWaySyncer(
        sync_data::Entry& syncContent,
        protocol::Client& protocolClient,
        conflict::Resolver& resolver) :
            FileSyncer{syncContent, protocolClient, resolver} {

    }

    void OneWaySyncer::doUpdate(sync_data::Entry* entry) {
        DEBUG_ENTER();

        if (entry) {
            if (fileExistsLocally(entry)) {
                if (fileExistsRemotely(entry)) {
                    getResolver().resolve(entry);
                } else {
                    getProtocolClient().upload(entry->getPath(), entry->getRemotePath());
                }
            } else {
                if (fileExistsRemotely(entry)) {
                    getProtocolClient().download(entry->getPath(), entry->getRemotePath());
                } else {
                    Logger::getInstance().log(LogDomain::Warning, 
                        "File '" + entry->getPath().string() + "' does "\
                        "neither exist locally nor remotely.");
                }
            }
            entry->setSyncInProgress();
            entry->resetChanged();
        }

        DEBUG_EXIT();
    }

}