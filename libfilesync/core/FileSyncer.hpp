#ifndef LIBFILESYNC_CORE_FILE_SYNCER_HPP
#define LIBFILESYNC_CORE_FILE_SYNCER_HPP

#include <libfilesync/data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>

#include <functional>
#include <vector>

namespace filesync::core {

    /**
     * @brief File syncer core base class to perform the sync
     * 
     * Patterns:
     *  - Template
     *  - Observer
     */  
    class FileSyncer : public Observer<Entry> {

        public:     
            FileSyncer(
                Entry& syncContent,
                ProtocolClient& protocolClient);

        protected:
            ProtocolClient& getProtocolClient();

        private:
            std::reference_wrapper<ProtocolClient> protocolClient;

            void doUpdate(Entry* entry = nullptr) override;
            
            void resolve(Entry* entry);
            virtual void doResolve(Entry* entry) = 0;

            [[nodiscard]] bool fileExistsLocally(Entry* entry);
            [[nodiscard]] bool fileExistsRemotely(Entry* entry); 
    };

}

#endif