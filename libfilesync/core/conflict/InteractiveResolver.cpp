#include <libfilesync/core/conflict/InteractiveResolver.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/FileSyncMutex.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <iostream>
#include <string>

namespace filesync::core::conflict {

    InteractiveResolver::InteractiveResolver(
        protocol::Client& protocolClient) :
            InteractiveResolver(protocolClient,
                std::make_shared<FileSyncLocks>()) {

    }

    InteractiveResolver::InteractiveResolver(protocol::Client& protocolClient,
        std::shared_ptr<FileSyncLocks> locks) :
            Resolver{protocolClient, locks} {

    }

    void InteractiveResolver::doResolve(sync_data::Entry* entry) {
        actions.at(askUser(*entry))(*entry);
    }

    ResolveOption InteractiveResolver::askUser(sync_data::Entry& entry) {
        printAskUserMessage(entry);
        return (charToResolveOption(readUserInput()));
    }

    void InteractiveResolver::printAskUserMessage(sync_data::Entry& entry) {
        std::cout << "------------" << std::endl;
        std::cout << "Sync conflict detected: " << std::endl;
        std::cout << "  Local file: " << entry.getAbsolutePath() << std::endl;
        std::cout << " Remote file: " << entry.getRemotePath() << std::endl;
        std::cout << "Please choose" << std::endl;
        std::cout << " c: to cancel" << std::endl;
        std::cout << " l: to upload the local file and overwrite the remote file" << std::endl;
        std::cout << " r: to download the remote file and overwrite the local file" << std::endl;
    }

    char InteractiveResolver::readUserInput() {
        char input;
        DEBUG("Retrieving mutex for 'std::cin'");
        std::scoped_lock lock(getLocks().getMutex(MutexType::stdCin));
        std::cin >> input;
        DEBUG("Releasing mutex for 'std::cin'");
        return input;
    }

    ResolveOption InteractiveResolver::charToResolveOption(char in) {
        switch (in) {
            case 'c':
                return ResolveOption::cancel;
            case 'l':
                return ResolveOption::useLocal;
            case 'r':
                return ResolveOption::useRemote;
            default:
                throw FileSyncException("Unknown option '" + std::to_string(in) + "'",
                    __FILE__, __LINE__);
        }
    }

}