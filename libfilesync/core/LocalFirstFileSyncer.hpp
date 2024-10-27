#ifndef LIBFILESYNC_CORE_LOCAL_FIRST_FILE_SYNCER_HPP
#define LIBFILESYNC_CORE_LOCAL_FIRST_FILE_SYNCER_HPP

#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>

namespace filesync::core {

    class LocalFirstFileSyncer : public FileSyncer {

        public:
            LocalFirstFileSyncer(
                Entry& syncContent,
                ProtocolClient& protocolClient);

        private:
            void doResolve(Entry* entry) override;

    };

}

#endif