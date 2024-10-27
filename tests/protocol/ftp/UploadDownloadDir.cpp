#include <tests/protocol/ftp/UploadDownloadDir.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <iostream>
#include <filesystem>

using namespace filesync::utility;

namespace filesync::integrationtest::protocol::ftp {

    UploadDownloadDir::UploadDownloadDir(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            ServerAccess(testName, server, pathOnServer),
            dirName1{"dir1"},
            dirName2{"dir2"} {

    }

    void UploadDownloadDir::setup() {
        std::filesystem::create_directory(dirName1);
    }

    void UploadDownloadDir::perform() {            
        FtpClient ftpClient(server, pathOnServer);
      
        ftpClient.upload(dirName1);
        ftpClient.download(dirName2, dirName1);
    }

    void UploadDownloadDir::evaluate() {
        FtpClient ftpClient(server, pathOnServer);
      
        if (!ftpClient.existsOnServer(dirName1)) {
            throw FileSyncException("Directory '" + dirName1 \
                + "' was not reported existent on server.",
                __FILE__, __LINE__);
        }

        if (!std::filesystem::is_directory(dirName2)) {
            throw FileSyncException("Local directory '" + dirName2 + "' does not exist.",
                __FILE__, __LINE__);            
        }
    }

    void UploadDownloadDir::cleanUp() {
        FtpClient ftpClient(server, pathOnServer);

        ftpClient.deleteOnServer(dirName1);
    }

}