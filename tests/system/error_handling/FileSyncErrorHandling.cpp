#include <tests/system/error_handling/FileSyncErrorHandling.hpp>
#include <libfilesync/FileSync.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/data/Data.hpp>

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;
namespace filesync::integration_test::system::error_handling {

    FileSyncErrorHandling::FileSyncErrorHandling(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest(testName),
            server{server},
            pathOnServer{pathOnServer} {

        TestCase missingSyncContent {
            .name = "Test file sync with missing sync content",
            .perform = std::bind(&FileSyncErrorHandling::performMissingSyncContent, this),
            .evaluate = std::bind(&FileSyncErrorHandling::evaluateMissingSyncContent, this)
        };

        addTestCase(missingSyncContent);

    }

    void FileSyncErrorHandling::setup() {
        prepareFileSystem();
    }

    void FileSyncErrorHandling::prepareFileSystem() {
        std::filesystem::create_directory("dir1");
        data::createFile("dir1/file1");
        data::createFile("dir1/file2");
        std::filesystem::create_directory("dir1/dir2");
        data::createFile("dir1/dir2/file3");
        data::createFile("dir1/dir2/file4");
        data::createFile("dir1/dir2/file1");
    }

    void FileSyncErrorHandling::performMissingSyncContent() {
        filesync::FileSync fileSync;

        fileSync.setServer(server);
        fileSync.setRemoteRoot(pathOnServer);

        fileSync.setConflictResolveStrategy(ConflictResolveStrategy::LocalFirst);
        fileSync.setProtocol(ProtocolType::FTP);
        fileSync.setSyncStrategy(SyncStrategy::UnBuffered);
        fileSync.setSyncInvertal(1s);

        exceptionThrown = false;
        try {
            fileSync.startSyncing(nullptr);

            std::this_thread::sleep_for(3s);
            
            fileSync.stopSyncing();
        } catch (...) {
            exceptionThrown = true;
        }

    }

    void FileSyncErrorHandling::evaluateMissingSyncContent() {
        if (exceptionThrown == false) {
            throw FileSyncException("Expected an exception on missing sync content",
            __FILE__, __LINE__);            
        }
    }

}