#ifndef LIBFILESYNC_CORE_CONFLICT_LOCAL_FIRST_RESOLVER_HPP
#define LIBFILESYNC_CORE_CONFLICT_LOCAL_FIRST_RESOLVER_HPP

#include <libfilesync/core/conflict/Resolver.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>

namespace filesync::core::conflict {

    class LocalFirstResolver : public Resolver {

        public:
            explicit LocalFirstResolver(ProtocolClient& protocolClient);

        private:
            void doResolve(sync_data::Entry* entry) override;

    };

}

#endif