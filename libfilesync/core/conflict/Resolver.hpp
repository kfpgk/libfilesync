#ifndef LIBFILESYNC_CORE_CONFLICT_RESOLVER_HPP
#define LIBFILESYNC_CORE_CONFLICT_RESOLVER_HPP

#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/protocol/Client.hpp>
#include <libfilesync/FileSyncLocks.hpp>

#include <memory>
namespace filesync::core::conflict {

    class Resolver {

        public:     
            explicit Resolver(protocol::Client& protocolClient);
            Resolver(protocol::Client& protocolClient,
                std::shared_ptr<FileSyncLocks> locks);
            virtual ~Resolver() = default;
            
            void resolve(sync_data::Entry* entry);
            
        protected:
            protocol::Client& getProtocolClient();
            FileSyncLocks& getLocks();
        
        private:
            protocol::Client& protocolClient;
            std::shared_ptr<FileSyncLocks> locks;
                
            virtual void doResolve(sync_data::Entry* entry) = 0;        

    };

}

#endif