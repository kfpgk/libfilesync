#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <memory>
#include <iostream>

using namespace filesync::utility;

namespace filesync::core {

    FileSyncer::FileSyncer(
        Entry& syncContent,
        ProtocolClient& protocolClient) :
            protocolClient{protocolClient} {

        syncContent.registerObserver(*this);
    }

    void FileSyncer::doUpdate(Entry* entry) {
        if (entry) {
            if (fileExistsLocally(entry)) {
                if (fileExistsRemotely(entry)) {
                    resolve(entry);
                } else {
                    protocolClient.get().upload(entry->getPath(), entry->getRemotePath());
                }
            } else {
                if (fileExistsRemotely(entry)) {
                    protocolClient.get().download(entry->getPath(), entry->getRemotePath());
                } else {
                    Logger::getInstance().log(LogDomain::Warning, 
                        "File '" + entry->getPath().string() + "' does \
                        neither exist locally nor remotely.");
                }
            }
            entry->resetChanged();
        }
    }

    void FileSyncer::resolve(Entry* entry) {
        doResolve(entry);
    }

    bool FileSyncer::fileExistsLocally(Entry* entry) {
        return entry->validate();
    }

    bool FileSyncer::fileExistsRemotely(Entry* entry) {
        return protocolClient.get().existsOnServer(entry->getRemotePath());
    }

    ProtocolClient& FileSyncer::getProtocolClient() {
        return protocolClient;
    }

}