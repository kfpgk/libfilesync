#include <tests/protocol/ftp/ServerAccess.hpp>

namespace filesync::integration_test::protocol::ftp {

    ServerAccess::ServerAccess(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest(testName),
            server{server},
            pathOnServer{pathOnServer} {

    }

    void ServerAccess::setUploadFileName(const std::string& fileName) {
        this->uploadFileName = fileName;
    }

    void ServerAccess::setDownloadFileName(const std::string& fileName) {
        this->downloadFileName = fileName;
    }

    void ServerAccess::setRemoteFileName(const std::string& fileName) {
        this->remoteFileName = fileName;
    }

}