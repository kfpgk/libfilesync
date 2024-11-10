#ifndef LIBFILESYNC_CORE_FILE_SYNCER_HPP
#define LIBFILESYNC_CORE_FILE_SYNCER_HPP

#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/core/conflict/Resolver.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>
#include <libfilesync/FileSyncLocks.hpp>

#include <memory>

namespace filesync::core {

    /**
     * @brief File syncer core base class to perform the sync
     * 
     * Patterns:
     *  - Template
     *  - Observer
     */  
    class FileSyncer : public Observer<sync_data::Entry> {

        public:     
            FileSyncer(
                sync_data::Entry& syncContent,
                ProtocolClient& protocolClient,
                conflict::Resolver& resolver);

            FileSyncer(
                sync_data::Entry& syncContent,
                ProtocolClient& protocolClient,
                conflict::Resolver& resolver,
                std::shared_ptr<FileSyncLocks> locks);

        protected:           
            ProtocolClient& getProtocolClient();
            conflict::Resolver& getResolver();
            FileSyncLocks& getLocks();
        
            [[nodiscard]] bool fileExistsLocally(sync_data::Entry* entry);
            [[nodiscard]] bool fileExistsRemotely(sync_data::Entry* entry);

        private: 
            ProtocolClient& protocolClient;
            conflict::Resolver& resolver;
            std::shared_ptr<FileSyncLocks> locks;
            
            void doUpdate(sync_data::Entry* entry = nullptr) override = 0;

    };

}

#endif