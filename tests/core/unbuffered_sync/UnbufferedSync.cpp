#include <tests/core/unbuffered_sync/UnbufferedSync.hpp>
#include <libfilesync/core/UnbufferedSyncer.hpp>
#include <libfilesync/core/conflict/LocalFirstResolver.hpp>
#include <libfilesync/core/conflict/RemoteFirstResolver.hpp>
#include <libfilesync/core/sync_data/File.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <functional>
#include <fstream>

namespace filesync::integration_test::core::unbuffered_sync {

    UnbufferedSync::UnbufferedSync(const std::string& name,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest{name},
            server{server},
            pathOnServer{pathOnServer},
            file1Name{"file1"},
            file1Content{"file1 content"},
            file2Name{"file2"},
            file2Content{"file2 content"} {

        TestCase initialSyncWithLocalOnly {
            .name = "Test initial sync cycle with only a local but no remote file",
            .perform = std::bind(&UnbufferedSync::performInitialSyncWithLocalOnly, this),
            .evaluate = std::bind(&UnbufferedSync::evaluateInitialSyncWithLocalOnly, this)
        };
        addTestCase(initialSyncWithLocalOnly);

        TestCase initialSyncWithRemoteOnly {
            .name = "Test initial sync cycle with only a remote but no local file",
            .perform = std::bind(&UnbufferedSync::performInitialSyncWithRemoteOnly, this),
            .evaluate = std::bind(&UnbufferedSync::evaluateInitialSyncWithRemoteOnly, this)
        };
        addTestCase(initialSyncWithRemoteOnly);

        TestCase initialSyncWithExistingLocalAndRemote {
            .name = "Test initial sync cycle with both a local and remote file",
            .perform = std::bind(
                &UnbufferedSync::performInitialSyncWithExistingLocalAndRemote, this),
            .evaluate = std::bind(
                &UnbufferedSync::evaluateInitialSyncWithExistingLocalAndRemote, this)
        };
        addTestCase(initialSyncWithExistingLocalAndRemote);

        TestCase syncWithUnchangedLocalAndChangedRemote {
            .name = "Test syncing with no changes to local file and with changed remote file",
            .perform = std::bind(
                &UnbufferedSync::performSyncWithUnchangedLocalAndChangedRemoteFile, this),
            .evaluate = std::bind(
                &UnbufferedSync::evaluateSyncWithUnchangedLocalAndChangedRemoteFile, this)
        };
        addTestCase(syncWithUnchangedLocalAndChangedRemote);

        TestCase syncWithChangedLocal {
            .name = "Test syncing with no changes to local file",
            .perform = std::bind(&UnbufferedSync::performSyncWithChangedLocalFile, this),
            .evaluate = std::bind(&UnbufferedSync::evaluateSyncWithChangedLocalFile, this)
        };
        addTestCase(syncWithChangedLocal);

    }

    void UnbufferedSync::performInitialSyncWithLocalOnly() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);

        if (proto.existsOnServer(file1Name)) {
            proto.deleteOnServer(file1Name);
        }

        filesync::core::conflict::RemoteFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::UnbufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void UnbufferedSync::evaluateInitialSyncWithLocalOnly() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        if (!data::areEqual("verify", file1Name)) {
            throw FileSyncException("Upload synced file does not match after download.",
                __FILE__, __LINE__);
        }
    }

    void UnbufferedSync::performInitialSyncWithRemoteOnly() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }


        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file1Name);
        std::remove(file1Name.c_str());

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::UnbufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void UnbufferedSync::evaluateInitialSyncWithRemoteOnly() {
        std::ifstream file1(file1Name);
        if (!file1.is_open()) {
            throw FileSyncException("Could not open expected downloaded file.",
                __FILE__, __LINE__);            
        }
        std::string line1;
        std::getline(file1, line1);
        if (file1Content != line1) {
            throw FileSyncException("Content of downloaded file did not match expectation.",
                __FILE__, __LINE__);
        }
    }

    void UnbufferedSync::performInitialSyncWithExistingLocalAndRemote() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
            std::ofstream file2(file2Name);
            file2 << file2Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file2Name, file1Name);

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::UnbufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void UnbufferedSync::evaluateInitialSyncWithExistingLocalAndRemote() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        if (!data::areEqual("verify", file1Name)) {
            throw FileSyncException("Upload synced file does not match after overwrite.",
                __FILE__, __LINE__);
        }
    }

    void UnbufferedSync::performSyncWithUnchangedLocalAndChangedRemoteFile() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
            std::ofstream file2(file2Name);
            file2 << file2Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file2Name, file1Name);

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::UnbufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);

        proto.upload(file2Name, file1Name);
        syncer.update(&file);
    }

    void UnbufferedSync::evaluateSyncWithUnchangedLocalAndChangedRemoteFile() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        if (!data::areEqual("verify", file2Name)) {
            throw FileSyncException("Synced local file does not match remote file.",
                __FILE__, __LINE__);
        }
    }

    void UnbufferedSync::performSyncWithChangedLocalFile() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
            std::ofstream file2(file2Name);
            file2 << file2Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file2Name, file1Name);

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::UnbufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);

        proto.upload(file2Name, file1Name);

        {
            std::ofstream file1(file1Name);
            file1 << "New file 1 content";
        }
        syncer.update(&file);
    }

    void UnbufferedSync::evaluateSyncWithChangedLocalFile() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        std::ifstream verifyFile("verify");
        if (!verifyFile.is_open()) {
            throw FileSyncException("Could not open expected downloaded file.",
                __FILE__, __LINE__);            
        }
        std::string line;
        std::getline(verifyFile, line);
        if ("New file 1 content" != line) {
            throw FileSyncException("Content of downloaded file did not match expectation.",
                __FILE__, __LINE__);
        }
    }

}