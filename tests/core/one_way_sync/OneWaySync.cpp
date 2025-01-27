#include <tests/core/one_way_sync/OneWaySync.hpp>
#include <libfilesync/core/OneWaySyncer.hpp>
#include <libfilesync/core/conflict/LocalFirstResolver.hpp>
#include <libfilesync/core/conflict/RemoteFirstResolver.hpp>
#include <libfilesync/core/sync_data/File.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <functional>
#include <fstream>

namespace filesync::integration_test::core::one_way_sync {

    OneWaySync::OneWaySync(const std::string& name,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest{name},
            server{server},
            pathOnServer{pathOnServer},
            file1Name{"file1"},
            file1Content{"file1 content"},
            file2Name{"file2"},
            file2Content{"file2 content"} {

        TestCase uploadSync {
            .name = "Test one way syncing in upload direction",
            .perform = std::bind(&OneWaySync::performUploadSync, this),
            .evaluate = std::bind(&OneWaySync::evaluateUploadSync, this)
        };
        addTestCase(uploadSync);

        TestCase uploadOverwrite {
            .name = "Test one way syncing in upload direction including overwriting remote file",
            .perform = std::bind(&OneWaySync::performUploadOverwrite, this),
            .evaluate = std::bind(&OneWaySync::evaluateUploadOverwrite, this)
        };
        addTestCase(uploadOverwrite);

        TestCase uploadSyncWithRemoteFileOnly {
            .name = "Test one way syncing in upload direction with missing remote file",
            .perform = std::bind(&OneWaySync::performUploadSyncWithRemoteFileOnly, this),
            .evaluate = std::bind(&OneWaySync::evaluateUploadSyncWithRemoteFileOnly, this)
        };
        addTestCase(uploadSyncWithRemoteFileOnly);

        TestCase uploadSyncWithMissingFiles {
            .name = "Test one way syncing in upload direction with missing files",
            .perform = std::bind(&OneWaySync::performUploadSyncWithMissingFiles, this),
            .evaluate = std::bind(&OneWaySync::evaluateUploadSyncWithMissingFiles, this)
        };
        addTestCase(uploadSyncWithMissingFiles);

        TestCase downloadSync {
            .name = "Test one way syncing in download direction",
            .perform = std::bind(&OneWaySync::performDownloadSync, this),
            .evaluate = std::bind(&OneWaySync::evaluateDownloadSync, this)
        };
        addTestCase(downloadSync);

        TestCase downloadOverwrite {
            .name = "Test one way syncing in download direction including overwriting local file",
            .perform = std::bind(&OneWaySync::performDownloadOverwrite, this),
            .evaluate = std::bind(&OneWaySync::evaluateDownloadOverwrite, this)
        };
        addTestCase(downloadOverwrite);

        TestCase downloadSyncWithLocalFileOnly {
            .name = "Test one way syncing in download direction with missing remote file",
            .perform = std::bind(&OneWaySync::performDownloadSyncWithLocalFileOnly, this),
            .evaluate = std::bind(&OneWaySync::evaluateDownloadSyncWithLocalFileOnly, this)
        };
        addTestCase(downloadSyncWithLocalFileOnly);

        TestCase downloadSyncWithMissingFiles {
            .name = "Test one way syncing in download direction with missing files",
            .perform = std::bind(&OneWaySync::performDownloadSyncWithMissingFiles, this),
            .evaluate = std::bind(&OneWaySync::evaluateDownloadSyncWithMissingFiles, this)
        };
        addTestCase(downloadSyncWithMissingFiles);

    }

    void OneWaySync::performUploadSync() {
        
        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);
        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::OneWaySyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void OneWaySync::evaluateUploadSync() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        if (!data::areEqual("verify", file1Name)) {
            throw FileSyncException("Upload synced file does not match after download.",
                __FILE__, __LINE__);
        }
    }
    

    void OneWaySync::performUploadOverwrite() {

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
        filesync::core::OneWaySyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }
    
    void OneWaySync::evaluateUploadOverwrite() {
        filesync::protocol::FtpClient proto(server, pathOnServer);
        proto.download("verify", file1Name);

        if (!data::areEqual("verify", file1Name)) {
            throw FileSyncException("Upload synced file does not match after overwrite.",
                __FILE__, __LINE__);
        }
    }

    void OneWaySync::performUploadSyncWithRemoteFileOnly() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
        }


        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file1Name);
        std::remove(file1Name.c_str());

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::OneWaySyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void OneWaySync::evaluateUploadSyncWithRemoteFileOnly() {
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

    void OneWaySync::performUploadSyncWithMissingFiles() {
        filesync::protocol::FtpClient proto(server, pathOnServer);

        if (proto.existsOnServer(file1Name)) {
            proto.deleteOnServer(file1Name);
        }
        std::remove(file1Name.c_str());

        filesync::core::conflict::LocalFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::OneWaySyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void OneWaySync::evaluateUploadSyncWithMissingFiles() {
        filesync::protocol::FtpClient proto(server, pathOnServer);

        if (proto.existsOnServer(file1Name)) {
            throw FileSyncException("File found on server, but no sync should have been performed.",
                __FILE__, __LINE__);            
        }
        if (std::filesystem::exists(file1Name)) {
            throw FileSyncException("Local file found, but no sync should have been performed.",
                __FILE__, __LINE__);                
        }       
    }

    void OneWaySync::performDownloadSync() {

        {
            remove(file1Name.c_str());
            std::ofstream file2(file2Name);
            file2 << file2Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file2Name, file1Name);

        filesync::core::conflict::RemoteFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::OneWaySyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }
    
    void OneWaySync::evaluateDownloadSync() {
        if (!data::areEqual(file1Name, file2Name)) {
            throw FileSyncException("Upload synced file does not match after download.",
                __FILE__, __LINE__);
        }
    }
    

    void OneWaySync::performDownloadOverwrite() {

        {
            std::ofstream file1(file1Name);
            file1 << file1Content;
            std::ofstream file2(file2Name);
            file2 << file2Content;
        }

        filesync::protocol::FtpClient proto(server, pathOnServer);

        proto.upload(file2Name, file1Name);

        filesync::core::conflict::RemoteFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::OneWaySyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }
    
    void OneWaySync::evaluateDownloadOverwrite() {
        if (!data::areEqual(file1Name, file2Name)) {
            throw FileSyncException("Upload synced file does not match after download.",
                __FILE__, __LINE__);
        }
    }
            
    void OneWaySync::performDownloadSyncWithLocalFileOnly() {

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
        filesync::core::OneWaySyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void OneWaySync::evaluateDownloadSyncWithLocalFileOnly() {
        filesync::protocol::FtpClient proto(server, pathOnServer);

        if (!proto.existsOnServer(file1Name)) {
            throw FileSyncException("Could not find remote file on server, but expected local" \
                "file to have been uploaded.",
                __FILE__, __LINE__);
        }
    }

    void OneWaySync::performDownloadSyncWithMissingFiles() {
        filesync::protocol::FtpClient proto(server, pathOnServer);

        if (proto.existsOnServer(file1Name)) {
            proto.deleteOnServer(file1Name);
        }
        std::remove(file1Name.c_str());

        filesync::core::conflict::RemoteFirstResolver resolver(proto);
        filesync::core::sync_data::File file(file1Name);
        filesync::core::OneWaySyncer syncer(file, proto, resolver);
    
        syncer.update(&file);
    }

    void OneWaySync::evaluateDownloadSyncWithMissingFiles() {
        filesync::protocol::FtpClient proto(server, pathOnServer);

        if (proto.existsOnServer(file1Name)) {
            throw FileSyncException("File found on server, but no sync should have been performed.",
                __FILE__, __LINE__);            
        }
        if (std::filesystem::exists(file1Name)) {
            throw FileSyncException("Local file found, but no sync should have been performed.",
                __FILE__, __LINE__);                
        }            
    }

}