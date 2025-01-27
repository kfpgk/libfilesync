#include <tests/core/buffered_sync/BufferedSync.hpp>
#include <libfilesync/core/BufferedSyncer.hpp>
#include <libfilesync/core/conflict/LocalFirstResolver.hpp>
#include <libfilesync/core/conflict/RemoteFirstResolver.hpp>
#include <libfilesync/core/sync_data/File.hpp>
#include <libfilesync/core/sync_data/buffer/Buffer.hpp>
#include <libfilesync/core/sync_data/buffer/CharArrayMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <functional>
#include <fstream>
#include <memory>

namespace filesync::integration_test::core::buffered_sync {

    BufferedSync::BufferedSync(const std::string& name,
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
            .perform = std::bind(&BufferedSync::performInitialSyncWithLocalOnly, this),
            .evaluate = std::bind(&BufferedSync::evaluateInitialSyncWithLocalOnly, this)
        };
        addTestCase(initialSyncWithLocalOnly);

        TestCase initialSyncWithRemoteOnly {
            .name = "Test initial sync cycle with only a remote but no local file",
            .perform = std::bind(&BufferedSync::performInitialSyncWithRemoteOnly, this),
            .evaluate = std::bind(&BufferedSync::evaluateInitialSyncWithRemoteOnly, this)
        };
        addTestCase(initialSyncWithRemoteOnly);

        TestCase initialSyncWithExistingLocalAndRemote {
            .name = "Test initial sync cycle with both a local and remote file",
            .perform = std::bind(
                &BufferedSync::performInitialSyncWithExistingLocalAndRemote, this),
            .evaluate = std::bind(
                &BufferedSync::evaluateInitialSyncWithExistingLocalAndRemote, this)
        };
        addTestCase(initialSyncWithExistingLocalAndRemote);

        TestCase syncWithUnchangedLocalAndChangedRemote {
            .name = "Test syncing with no changes to local file and with changed remote file",
            .perform = std::bind(
                &BufferedSync::performSyncWithUnchangedLocalAndChangedRemoteFile, this),
            .evaluate = std::bind(
                &BufferedSync::evaluateSyncWithUnchangedLocalAndChangedRemoteFile, this)
        };
        addTestCase(syncWithUnchangedLocalAndChangedRemote);

        TestCase syncWithLocalAndRemoteChanges {
            .name = "Test syncing with changes to local and remote file",
            .perform = std::bind(&BufferedSync::performSyncWithLocalAndRemoteChanges, this),
            .evaluate = std::bind(&BufferedSync::evaluateSyncWithLocalAndRemoteChanges, this)
        };
        addTestCase(syncWithLocalAndRemoteChanges);

        TestCase syncWithFileBuffers {
            .name = "Test syncing with file buffers",
            .perform = std::bind(&BufferedSync::performSyncWithFileBuffers, this),
            .evaluate = std::bind(&BufferedSync::evaluateSyncWithFileBuffers, this)
        };
        addTestCase(syncWithFileBuffers);

        TestCase syncWithMemoryBuffers {
            .name = "Test syncing with in-memory buffers",
            .perform = std::bind(&BufferedSync::performSyncWithMemoryBuffers, this),
            .evaluate = std::bind(&BufferedSync::evaluateSyncWithMemoryBuffers, this)
        };
        addTestCase(syncWithMemoryBuffers);

    }

    void BufferedSync::performInitialSyncWithLocalOnly() {

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
        filesync::core::BufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void BufferedSync::evaluateInitialSyncWithLocalOnly() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        if (!data::areEqual("verify", file1Name)) {
            throw FileSyncException("Upload synced file does not match after download.",
                __FILE__, __LINE__);
        }
    }

    void BufferedSync::performInitialSyncWithRemoteOnly() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }


        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file1Name);
        std::remove(file1Name.c_str());

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::BufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void BufferedSync::evaluateInitialSyncWithRemoteOnly() {
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

    void BufferedSync::performInitialSyncWithExistingLocalAndRemote() {

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
        filesync::core::BufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void BufferedSync::evaluateInitialSyncWithExistingLocalAndRemote() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        if (!data::areEqual("verify", file1Name)) {
            throw FileSyncException("Upload synced file does not match after overwrite.",
                __FILE__, __LINE__);
        }
    }

    void BufferedSync::performSyncWithUnchangedLocalAndChangedRemoteFile() {

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
        filesync::core::BufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);

        proto.upload(file2Name, file1Name);
        syncer.update(&file);
    }

    void BufferedSync::evaluateSyncWithUnchangedLocalAndChangedRemoteFile() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        if (!data::areEqual("verify", file2Name)) {
            throw FileSyncException("Synced local file does not match remote file.",
                __FILE__, __LINE__);
        }
    }

    void BufferedSync::performSyncWithLocalAndRemoteChanges() {

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
        filesync::core::BufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);

        proto.upload(file2Name, file1Name);

        {
            std::ofstream file1(file1Name);
            file1 << "New file 1 content";
        }
        syncer.update(&file);
    }

    void BufferedSync::evaluateSyncWithLocalAndRemoteChanges() {
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

    void BufferedSync::performSyncWithFileBuffers() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
            std::ofstream file2(file2Name);
            file2 << file2Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file2Name, file1Name);

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name,
            std::make_unique<filesync::core::sync_data::buffer::FileBuffer>(),
            std::make_unique<filesync::core::sync_data::buffer::FileBuffer>());
        filesync::core::BufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);

        proto.upload(file2Name, file1Name);

        {
            std::ofstream file1(file1Name);
            file1 << "New file 1 content";
        }
        syncer.update(&file);
    }

    void BufferedSync::evaluateSyncWithFileBuffers() {
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

    void BufferedSync::performSyncWithMemoryBuffers() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
            std::ofstream file2(file2Name);
            file2 << file2Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file2Name, file1Name);

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name,
            std::make_unique<filesync::core::sync_data::buffer::ProtocolMemoryBuffer>(),
            std::make_unique<filesync::core::sync_data::buffer::CharArrayMemoryBuffer>());
        filesync::core::BufferedSyncer syncer(file, proto, resolver);
    
        syncer.update(&file);

        proto.upload(file2Name, file1Name);

        {
            std::ofstream file1(file1Name);
            file1 << "New file 1 content";
        }
        syncer.update(&file);
    }

    void BufferedSync::evaluateSyncWithMemoryBuffers() {
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