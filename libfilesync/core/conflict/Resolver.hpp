#ifndef LIBFILESYNC_CORE_CONFLICT_RESOLVER_HPP
#define LIBFILESYNC_CORE_CONFLICT_RESOLVER_HPP

#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>

#include <functional>

namespace filesync::core::conflict {

    class Resolver {

        public:     
            explicit Resolver(ProtocolClient& protocolClient);
            virtual ~Resolver() = default;
            
            void resolve(sync_data::Entry* entry);
            
        protected:
            ProtocolClient& getProtocolClient();
        
        private:
            std::reference_wrapper<ProtocolClient> protocolClient;
                
            virtual void doResolve(sync_data::Entry* entry) = 0;        

    };

}

#endif