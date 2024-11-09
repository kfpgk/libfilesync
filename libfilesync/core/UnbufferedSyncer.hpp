#ifndef LIBFILESYNC_CORE_UNBUFFERED_SYNCER_HPP
#define LIBFILESYNC_CORE_UNBUFFERED_SYNCER_HPP

#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/core/conflict/Resolver.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>

namespace filesync::core {

    class UnbufferedSyncer : public FileSyncer {

        public:
            UnbufferedSyncer(
                sync_data::Entry& syncContent,
                ProtocolClient& protocolClient,
                conflict::Resolver& resolver);

        private:
            void doUpdate(sync_data::Entry* entry = nullptr) override;

    };

}

#endif