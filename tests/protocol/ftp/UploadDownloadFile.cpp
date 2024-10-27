#include <tests/protocol/ftp/UploadDownloadFile.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <iostream>
#include <fstream>

using namespace filesync::utility;

namespace filesync::integrationtest::protocol::ftp {

    UploadDownloadFile::UploadDownloadFile(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            ServerAccess(testName, server, pathOnServer) {

    }

    void UploadDownloadFile::setup() {
        std::ofstream uploadFile(uploadFileName);
        if (!uploadFile.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        uploadFile << testFileContent << std::endl;

        FtpClient ftpClient(server);
        ftpClient.setRemoteRootPath(pathOnServer);

        if (ftpClient.existsOnServer(remoteFileName)) {
            ftpClient.deleteOnServer(remoteFileName);
        }
    }

    void UploadDownloadFile::perform() {            
        FtpClient ftpClient(server);
        ftpClient.setRemoteRootPath(pathOnServer);
      
        ftpClient.upload(uploadFileName, remoteFileName);
        ftpClient.download(downloadFileName, remoteFileName);
    }

    void UploadDownloadFile::evaluate() {
        FtpClient ftpClient(server);
        ftpClient.setRemoteRootPath(pathOnServer);
      
        if (!ftpClient.existsOnServer(remoteFileName)) {
            throw FileSyncException("File '" + remoteFileName \
                + "' was not reported existent on server.",
                __FILE__, __LINE__);
        }

        std::ifstream uploadFile(uploadFileName);
        if (!uploadFile.is_open()) {
            throw FileSyncException("Cannot open '" + uploadFileName + "' for reading.",
                __FILE__, __LINE__);
        }
        std::ifstream download(downloadFileName);
        if (!download.is_open()) {
            throw FileSyncException("Cannot open '" + downloadFileName + "' for reading.",
                __FILE__, __LINE__);
        }
        std::string lineUpload;
        std::string lineDownload;
        std::getline(uploadFile, lineUpload);
        std::getline(download, lineDownload);
        if (testFileContent != lineUpload) {
            std::stringstream message;
            message << "Uploaded file content does not match expectation:" << std::endl;
            message << "Uploaded file content: " << std::endl;
            message << " '" << lineUpload << "'" << std::endl;
            message << "Expected content: " << std::endl;
            message << " '" << testFileContent << "'" << std::endl;
            throw FileSyncException(message.str(), __FILE__, __LINE__);
        }
        if (testFileContent != lineDownload) {
            std::stringstream message;
            message << "Downloaded file content does not match expectation." << std::endl;
            message << "Downloaded file content: " << std::endl;
            message << " '" << lineUpload << "'" << std::endl;
            message << "Expected content: " << std::endl;
            message << " '" << testFileContent << "'" << std::endl;
            throw FileSyncException(message.str(), __FILE__, __LINE__);
        }
        Logger::getInstance().log(LogDomain::Info,
            "Uploaded file content matches downloaded file content.");
        uploadFile.close();
        download.close();
    }

    /**
     * Clean up files, because if we use absolute paths for these
     * files, they might not be in the sandbox and therefor not
     * cleaned by base class.
     */
    void UploadDownloadFile::cleanUp() {
        int rc;
        rc = std::remove(uploadFileName.c_str());
        if (rc != 0) {
            std::cout << "Failed to delete '" << uploadFileName << "'" << std::endl;
        }
        rc = remove(downloadFileName.c_str());
        if (rc != 0) {
            std::cout << "Failed to delete '" << downloadFileName << "'" << std::endl;
        }
        FtpClient ftpClient(server);
        ftpClient.setRemoteRootPath(pathOnServer);
        ftpClient.deleteOnServer(remoteFileName);
    }

}