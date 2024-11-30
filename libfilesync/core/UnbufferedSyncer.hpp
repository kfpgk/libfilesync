#ifndef LIBFILESYNC_CORE_UNBUFFERED_SYNCER_HPP
#define LIBFILESYNC_CORE_UNBUFFERED_SYNCER_HPP

#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/core/conflict/Resolver.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/protocol/Client.hpp>

namespace filesync::core {

    /**
     * @brief File syncer without buffers
     * 
     * This syncer uploads if it detects local
     * changes to the file and downloads in any
     * other case.
     * 
     * This logic only begins to work when the 
     * sync is in progress. In the first/initial
     * sync cycle, behavior is defined by the
     * used conflict resolver if both local and
     * remote file exist.
     */
    class UnbufferedSyncer : public FileSyncer {

        public:
            UnbufferedSyncer(
                sync_data::Entry& syncContent,
                protocol::Client& protocolClient,
                conflict::Resolver& resolver);

        private:
            void doUpdate(sync_data::Entry* entry = nullptr) override;

    };

}

#endif