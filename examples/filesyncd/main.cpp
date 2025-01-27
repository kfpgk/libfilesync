#include <libfilesync/FileSync.hpp>
#include <libfilesync/FileSyncLocks.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <thread>
#include <chrono>

struct CmdLineArgs {
    static std::string localSyncPath;
    static std::string server;
    static std::string serverRoot;
};
std::string CmdLineArgs::localSyncPath = "";
std::string CmdLineArgs::server = "";
std::string CmdLineArgs::serverRoot = "";

void usage(const std::string& progName) {
    std::cout << "Usage:" << std::endl;
    std::cout << progName << " -l local-sync-entry -s remote-server -r remote-root" << std::endl;
}

[[nodiscard]] 
bool parseCmdLineArgs(const std::vector<std::string>& args) {
    
    std::filesystem::path progPath(args.front());

    for(auto& arg : args) {
        if (arg == "-h" || arg == "--help") {
            usage(progPath.filename());
            return false;
        } else if (arg == "-l") {
            CmdLineArgs::localSyncPath = *(&arg + 1);
        } else if (arg == "-s") {
            CmdLineArgs::server = *(&arg + 1);
        } else if (arg == "-r") {
            CmdLineArgs::serverRoot = *(&arg + 1);
        }
    }
    if (CmdLineArgs::localSyncPath.empty() || CmdLineArgs::server.empty() || CmdLineArgs::serverRoot.empty()) {
        usage(progPath.filename());
        return false;        
    } else {
        std::cout << "local-sync-entry: " << CmdLineArgs::localSyncPath << std::endl;
        std::cout << "remote-server:    " << CmdLineArgs::server << std::endl;
        std::cout << "remote-root:      " << CmdLineArgs::serverRoot << std::endl;
        return true;
    }
}

int main(int argc, char* argv[]) {

    std::vector<std::string> cmdLineArgs(argv, argv+argc);

    if (!parseCmdLineArgs(cmdLineArgs)) {
        return -1;
    }

    using namespace filesync;

    try {
        FileSync syncer;

        auto locks = std::make_shared<FileSyncLocks>();
        auto cinMutex = std::make_shared<std::mutex>();

        syncer.setProtocol(ProtocolType::FTP);
        syncer.setConflictResolveStrategy(ConflictResolveStrategy::Interactive);
        syncer.setSyncStrategy(SyncStrategy::Buffered);

        syncer.setServer(CmdLineArgs::server);
        syncer.setRemoteRoot(CmdLineArgs::serverRoot);
        syncer.setSyncContent(CmdLineArgs::localSyncPath);

        locks->addMutex(MutexType::stdCin, cinMutex);

        syncer.startSyncing(locks);
        
        using namespace std::chrono_literals;
        while(true) {
            std::this_thread::sleep_for(2s);
            {
                std::scoped_lock lock(*cinMutex);
                std::cin.ignore();
                if (std::cin.get() == '\n') break;
            }
        }
        syncer.stopSyncing();

    } catch (const FileSyncException& e) {
        std::cerr << e.what();
        return -1;
    }
    
    return 0;
}