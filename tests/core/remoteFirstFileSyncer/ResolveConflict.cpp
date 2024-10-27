#include <tests/core/remoteFirstFileSyncer/ResolveConflict.hpp>
#include <libfilesync/core/RemoteFirstFileSyncer.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <sstream>
#include <fstream>

namespace filesync::integrationtest::core::remote_first_file_syncer {

    ResolveConflict::ResolveConflict(
        const std::string& testName) :
            core::ResolveConflict(testName) {
      
    }

    ResolveConflict::ResolveConflict(
        const std::string& testName,
        std::unique_ptr<ProtocolClient> protocolClient) :
            core::ResolveConflict(testName, std::move(protocolClient)) {
      
    }

    void ResolveConflict::perform() {
        if (!entry || !protocolClient) {
            throw FileSyncException("Need Entry and protocol client to create\
                FileSyncer object()", __FILE__, __LINE__);
        }
        filesync::core::RemoteFirstFileSyncer fileSyncer(*entry, *protocolClient);
        fileSyncer.update(entry.get());
        
    }

    void ResolveConflict::evaluate() {

        std::ifstream localFile(localFileName);
        if (!localFile.is_open()) {
            throw FileSyncException("Cannot open '" + localFileName + "' for reading.",
                __FILE__, __LINE__);
        }
        std::string lineLocalFile;
        std::getline(localFile, lineLocalFile);
        if (lineLocalFile != remoteFileContent) {
            std::stringstream message;
            message << "Local file did not get updated with remote file content:" << std::endl;
            message << "         Content: '" << lineLocalFile << "'" << std::endl;
            message << "Expected content: '" << remoteFileContent << "'" << std::endl;
            throw FileSyncException(message.str(), __FILE__, __LINE__);
        }       

    }

}