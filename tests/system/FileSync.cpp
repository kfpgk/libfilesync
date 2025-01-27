#include <tests/system/FileSync.hpp>
#include <libfilesync/FileSync.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/data/Data.hpp>

#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
namespace filesync::integration_test::system {

    FileSync::FileSync(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest(testName),
            server{server},
            pathOnServer{pathOnServer} {

    }

    void FileSync::setup() {
        prepareFileSystem();
    }

    void FileSync::prepareFileSystem() {
        std::filesystem::create_directory("dir1");
        data::createFile("dir1/file1");
        data::createFile("dir1/file2");
        std::filesystem::create_directory("dir1/dir2");
        data::createFile("dir1/dir2/file3");
        data::createFile("dir1/dir2/file4");
        data::createFile("dir1/dir2/file1");
    }

    void FileSync::perform() {
        filesync::FileSync fileSync;

        fileSync.setServer(server);
        fileSync.setRemoteRoot(pathOnServer);
        fileSync.setSyncContent("dir1");

        fileSync.setConflictResolveStrategy(ConflictResolveStrategy::LocalFirst);
        fileSync.setProtocol(ProtocolType::FTP);
        fileSync.setSyncStrategy(SyncStrategy::UnBuffered);
        fileSync.setSyncInvertal(1s);

        fileSync.startSyncing(nullptr);

        std::this_thread::sleep_for(3s);
        
        fileSync.stopSyncing();

    }

    void FileSync::evaluate() {
        filesync::protocol::FtpClient client(server, pathOnServer);

        if (!client.existsOnServer("dir1")) {
            throw FileSyncException("Could not find 'dir1' on remote server",
            __FILE__, __LINE__);
        }
        if (!client.existsOnServer("dir1/file1")) {
            throw FileSyncException("Could not find 'dir1/file1' on remote server",
            __FILE__, __LINE__);            
        }
        if (!client.existsOnServer("dir1/file2")) {
            throw FileSyncException("Could not find 'dir1/file2' on remote server",
            __FILE__, __LINE__);            
        }
        if (!client.existsOnServer("dir1/dir2")) {
            throw FileSyncException("Could not find 'dir1/dir2' on remote server",
            __FILE__, __LINE__);            
        }
        if (!client.existsOnServer("dir1/dir2/file3")) {
            throw FileSyncException("Could not find 'dir1/file2/file3' on remote server",
            __FILE__, __LINE__);            
        }
        if (!client.existsOnServer("dir1/dir2/file4")) {
            throw FileSyncException("Could not find 'dir1/dir2/file4' on remote server",
            __FILE__, __LINE__);            
        }
        if (!client.existsOnServer("dir1/dir2/file1")) {
            throw FileSyncException("Could not find 'dir1/dir2/file1' on remote server",
            __FILE__, __LINE__);            
        }
    }

    void FileSync::cleanUp() {
        filesync::protocol::FtpClient client(server, pathOnServer);

        client.deleteOnServer("dir1/file1");
        client.deleteOnServer("dir1/file2");
        client.deleteOnServer("dir1/dir2/file3");
        client.deleteOnServer("dir1/dir2/file4");
        client.deleteOnServer("dir1/dir2/file1");
        client.deleteOnServer("dir1/dir2");
        client.deleteOnServer("dir1");

    }

}