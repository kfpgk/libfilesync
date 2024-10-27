#include <tests/core/localFirstFileSyncer/ResolveConflict.hpp>
#include <libfilesync/core/LocalFirstFileSyncer.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <sstream>
#include <fstream>

namespace filesync::integrationtest::core::local_first_file_syncer {

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
        filesync::core::LocalFirstFileSyncer fileSyncer(*entry, *protocolClient);
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
        if (lineLocalFile != localFileContent) {
            std::stringstream message;
            message << "Local file content unexpectedly changed:" << std::endl;
            message << "         Content: '" << lineLocalFile << "'" << std::endl;
            message << "Expected content: '" << localFileContent << "'" << std::endl;
            throw FileSyncException(message.str(), __FILE__, __LINE__);
        }    

    }

}