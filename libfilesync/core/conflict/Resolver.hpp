#ifndef LIBFILESYNC_CORE_CONFLICT_RESOLVER_HPP
#define LIBFILESYNC_CORE_CONFLICT_RESOLVER_HPP

#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>
#include <libfilesync/FileSyncLocks.hpp>

#include <memory>
namespace filesync::core::conflict {

    class Resolver {

        public:     
            explicit Resolver(ProtocolClient& protocolClient);
            Resolver(ProtocolClient& protocolClient,
                std::shared_ptr<FileSyncLocks> locks);
            virtual ~Resolver() = default;
            
            void resolve(sync_data::Entry* entry);
            
        protected:
            ProtocolClient& getProtocolClient();
            FileSyncLocks& getLocks();
        
        private:
            ProtocolClient& protocolClient;
            std::shared_ptr<FileSyncLocks> locks;
                
            virtual void doResolve(sync_data::Entry* entry) = 0;        

    };

}

#endif