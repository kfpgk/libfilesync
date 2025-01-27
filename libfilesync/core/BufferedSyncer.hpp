#ifndef LIBFILESYNC_CORE_BUFFERED_SYNCER_HPP
#define LIBFILESYNC_CORE_BUFFERED_SYNCER_HPP

#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/core/conflict/Resolver.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/protocol/Client.hpp>

namespace filesync::core {

    class BufferedSyncer : public FileSyncer {

        public:
            BufferedSyncer(
                sync_data::Entry& syncContent,
                protocol::Client& protocolClient,
                conflict::Resolver& resolver);

        private:
            void doUpdate(sync_data::Entry* entry = nullptr) override;

    };

}

#endif