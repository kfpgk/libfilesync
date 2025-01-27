#include <tests/core/conflict/ResolveConflict.hpp>
#include <tests/config/config.h>
#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/core/sync_data/File.hpp>

#include <fstream>
#include <cstdio>

namespace filesync::integration_test::core::conflict {

    ResolveConflict::ResolveConflict(const std::string& testName) :
        IntegrationTest(testName) {

        protocolClient = std::make_unique<protocol::FtpClient>(
            LIBFILESYNC_TEST_FTP_SERVER,
            LIBFILESYNC_TEST_FTP_SERVER_DIR);

    }

    ResolveConflict::ResolveConflict(const std::string& testName,
        std::unique_ptr<protocol::Client> protocolClient) :
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

        entry = std::make_unique<filesync::core::sync_data::File>(localFileName);
        entry->setRemoteEntry(remoteFileName);
    }

    void ResolveConflict::cleanUp() {
        protocolClient->deleteOnServer(remoteFileName);
    }

}