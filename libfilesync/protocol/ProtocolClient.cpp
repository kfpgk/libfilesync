#include <libfilesync/protocol/ProtocolClient.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <sstream>
#include <filesystem>

using namespace filesync::utility;

namespace filesync {

    ProtocolClient::ProtocolClient(const std::string& remoteRoot) :
        remoteRoot{remoteRoot} {

    }

    void ProtocolClient::download(const std::filesystem::path& local) {
        download(local, local);
    }

    void ProtocolClient::download(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        DEBUG_ENTER(); 

        std::stringstream message;
        message << "Download: '" << local.string() << "' << " << getCompleteRemoteFilePath(remote); 
        Logger::getInstance().log(LogDomain::Info, message.str());

        doDownload(local, remote);

        DEBUG_EXIT(); 
    }

    void ProtocolClient::upload(const std::filesystem::path& local) {
        upload(local, local);
    }

    void ProtocolClient::upload(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        DEBUG_ENTER();

        std::stringstream message;
        message << "Upload: '" << local.string() << "' >> " << getCompleteRemoteFilePath(remote); 
        Logger::getInstance().log(LogDomain::Info, message.str());

        doUpload(local, remote);

        DEBUG_EXIT(); 
    }

    bool ProtocolClient::existsOnServer(
        const std::filesystem::path& remote) {

        return doExistsOnServer(remote);
    }

    void ProtocolClient::deleteOnServer(
        const std::filesystem::path& remote) {

        DEBUG_ENTER();

        doDeleteOnServer(remote);

        DEBUG_EXIT();
    }

    void ProtocolClient::setRemoteRootPath(
        const std::filesystem::path& remoteRoot) {
        
        this->remoteRoot = remoteRoot;
    }

    std::filesystem::path ProtocolClient::getCompleteRemoteFilePath(
        const std::filesystem::path& remoteFile) const {

        using namespace std::filesystem;
        return remoteRoot.string() + path::preferred_separator + remoteFile.string();

    }

}