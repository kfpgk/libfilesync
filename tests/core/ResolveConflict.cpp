#include <tests/core/ResolveConflict.hpp>
#include <tests/config/config.h>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/data/Directory.hpp>
#include <libfilesync/data/File.hpp>

#include <fstream>
#include <cstdio>

namespace filesync::integrationtest::core {

    ResolveConflict::ResolveConflict(const std::string& testName) :
        IntegrationTest(testName) {

        protocolClient = std::make_unique<FtpClient>(
            LIBFILESYNC_TEST_FTP_SERVER,
            LIBFILESYNC_TEST_FTP_SERVER_DIR);

    }

    ResolveConflict::ResolveConflict(const std::string& testName,
        std::unique_ptr<ProtocolClient> protocolClient) :
        IntegrationTest(testName),
        protocolClient{std::move(protocolClient)} {

    }

    /**
     * @brief Setup for conflict resolving tests
     * 
     * This method creates a local file and uploads
     * a remote file to the server. These two have
     * different contents.
     * After complition, the "entry" member holds
     * a reference to the local file with a linkage
     * to the remote file. 
     */
    void ResolveConflict::setup() {
        std::ofstream localFile(localFileName);
        if (!localFile.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        localFile << localFileContent << std::endl;
        localFile.close();

        std::ofstream remoteFile(remoteFileName);
        if (!remoteFile.is_open()) {
            throw FileSyncException("Cannot open local file for writing.",
                __FILE__, __LINE__);
        }
        remoteFile << remoteFileContent << std::endl;
        remoteFile.close();

        protocolClient->upload(remoteFileName, remoteFileName);
        std::remove(remoteFileName.c_str());

        entry = std::make_unique<File>(localFileName);
        entry->setRemote(remoteFileName);
    }

    void ResolveConflict::cleanUp() {
        protocolClient->deleteOnServer(remoteFileName);
    }

}