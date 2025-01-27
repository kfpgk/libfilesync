#include <libfilesync/FileSync.test.hpp>
#include <libfilesync/FileSync.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <cassert>
#include <chrono>
#include <thread>

using namespace filesync::utility;
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {

    filesync::unit_test::FileSyncTest test;

    test.missing_sync_content();
    test.non_existent_sync_content();

    Logger::getInstance().log(LogDomain::TestResult, 
        "FileSync: passed", __FILE__, __LINE__);
    return 0;
}

namespace filesync::unit_test {

    void FileSyncTest::missing_sync_content() {

        filesync::FileSync fileSync;

        fileSync.setServer("does-not-matter");

        fileSync.setConflictResolveStrategy(ConflictResolveStrategy::LocalFirst);
        fileSync.setProtocol(ProtocolType::FTP);
        fileSync.setSyncStrategy(SyncStrategy::UnBuffered);
        fileSync.setSyncInvertal(1s);

        bool exceptionThrown = false;
        try {
            fileSync.startSyncing(nullptr);

            std::this_thread::sleep_for(3s);
            
            fileSync.stopSyncing();
        } catch(...) {
            exceptionThrown = true;
        }
        
        assert(exceptionThrown == true);
        Logger::getInstance().log(LogDomain::TestResult,
            "missing_sync_content() passed");
    }

    void FileSyncTest::non_existent_sync_content() {

        filesync::FileSync fileSync;

        bool exceptionThrown = false;
        try {
            fileSync.setSyncContent("dir1");
        } catch(...) {
            exceptionThrown = true;
        }
        
        assert(exceptionThrown == true);
        Logger::getInstance().log(LogDomain::TestResult,
            "non_existent_sync_content() passed");
    }

}