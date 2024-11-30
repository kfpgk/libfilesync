#include <tests/protocol/ftp/DeleteRemoteFile.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <fstream>

namespace filesync::integration_test::protocol::ftp {

    DeleteRemoteFile::DeleteRemoteFile(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            ServerAccess(testName, server, pathOnServer) {

    }

    void DeleteRemoteFile::setup() {
        std::ofstream uploadFile(uploadFileName);
        if (!uploadFile.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        uploadFile << "Test file content" << std::endl;

        filesync::protocol::FtpClient ftpClient(server);
        ftpClient.setRemoteRootPath(pathOnServer);

        ftpClient.upload(uploadFileName, remoteFileName);

        if (!ftpClient.existsOnServer(remoteFileName)) {
            throw FileSyncException("File '" + remoteFileName \
                + "' was not reported existent on server.",
                __FILE__, __LINE__);
        }
    }

    void DeleteRemoteFile::perform() {      
        filesync::protocol::FtpClient ftpClient(server);
        ftpClient.setRemoteRootPath(pathOnServer);

        ftpClient.deleteOnServer(remoteFileName);

        ftpClient.upload(uploadFileName, "other-name");
        ftpClient.download(downloadFileName, "other-name");

    }

    void DeleteRemoteFile::evaluate() {
        filesync::protocol::FtpClient ftpClient(server);
        ftpClient.setRemoteRootPath(pathOnServer);

        if (ftpClient.existsOnServer(remoteFileName)) {
            throw FileSyncException("File '" + remoteFileName \
                + "' still exists on server. Should have been deleted.",
                __FILE__, __LINE__);
        }
    }

    void DeleteRemoteFile::cleanUp() {
        filesync::protocol::FtpClient ftpClient(server);
        ftpClient.setRemoteRootPath(pathOnServer);

        ftpClient.deleteOnServer("other-name");
    }

}