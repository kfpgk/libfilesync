#ifndef LIBFILESYNC_CORE_CONFLICT_INTERACTIVE_RESOLVER_HPP
#define LIBFILESYNC_CORE_CONFLICT_INTERACTIVE_RESOLVER_HPP

#include <libfilesync/core/conflict/Resolver.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>
#include <libfilesync/FileSyncLocks.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <unordered_map>
#include <functional>

namespace filesync::core::conflict {

    enum class ResolveOption {cancel, useLocal, useRemote};

    class InteractiveResolver : public Resolver {

        public:
            InteractiveResolver(ProtocolClient& protocolClient);
            InteractiveResolver(ProtocolClient& protocolClient,
                std::shared_ptr<FileSyncLocks> locks);

        private:
            std::unordered_map<ResolveOption, std::function<void(sync_data::Entry&)>> actions
            {
                { ResolveOption::cancel, [this](sync_data::Entry& entry) {
                    DEBUG("User input to cancel sync detected. Exiting...");
                    exit(0);
                }},
                { ResolveOption::useLocal, [this](sync_data::Entry& entry) { 
                    DEBUG("User input to use local detected. Uploading...");
                    getProtocolClient().upload(entry.getPath(), entry.getRemotePath()); 
                }},
                { ResolveOption::useRemote, [this](sync_data::Entry& entry) {
                    DEBUG("User input to use remote detected. Downloading..."); 
                    getProtocolClient().download(entry.getPath(), entry.getRemotePath()); 
                }},
            };

            [[nodiscard]] ResolveOption askUser(sync_data::Entry& entry);
            void printAskUserMessage(sync_data::Entry& entry);
            [[nodiscard]] char readUserInput();
            [[nodiscard]] ResolveOption charToResolveOption(char in);

            void doResolve(sync_data::Entry* entry) override;

    };

}

#endif