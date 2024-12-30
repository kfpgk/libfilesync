#include <libfilesync/protocol/Client.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Debug.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <sstream>
#include <filesystem>

using namespace filesync::utility;

namespace filesync::protocol {

    Client::Client(const std::string& remoteRoot) :
        remoteRoot{remoteRoot} {

    }

    void Client::download(const std::filesystem::path& local) {
        download(local, local);
    }

    /*void Client::download(
        HandleType local,
        const std::filesystem::path& remote) {

        if (std::holds_alternative<std::reference_wrapper<std::filesystem::path>>(local)) {
            download(std::get<std::reference_wrapper<std::filesystem::path>>(local), remote);
        } else if (std::holds_alternative<
            std::reference_wrapper<std::unique_ptr<memory::Handle<char>>>>(local)) {
            download(std::get<
                std::reference_wrapper<std::unique_ptr<memory::Handle<char>>>>(local), remote);
        } else {
            throw FileSyncException("Cannot handle local handle type.",
                __FILE__, __LINE__);
        }

    }*/

    void Client::download(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        DEBUG_ENTER(); 

        std::stringstream message;
        message << "Download: '" << local.string() << "' << " << getCompleteRemoteFilePath(remote); 
        Logger::getInstance().log(LogDomain::Info, message.str());

        checkForEmptyPath(local);
        checkForEmptyPath(remote);
        doDownload(local, remote);

        DEBUG_EXIT(); 
    }

    void Client::download(
        std::unique_ptr<memory::Handle<char>>& local,
        const std::filesystem::path& remote) {

        DEBUG_ENTER(); 

        std::stringstream message;
        message << "Download: " << getCompleteRemoteFilePath(remote) << " to char buffer";
        Logger::getInstance().log(LogDomain::Info, message.str());

        checkForEmptyPath(remote);
        doDownload(local, remote);

        DEBUG_EXIT(); 

    }

    void Client::upload(const std::filesystem::path& local) {
        upload(local, local);
    }

    void Client::upload(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        DEBUG_ENTER();

        std::stringstream message;
        message << "Upload: '" << local.string() << "' >> " << getCompleteRemoteFilePath(remote); 
        Logger::getInstance().log(LogDomain::Info, message.str());

        checkForEmptyPath(local);
        checkForEmptyPath(remote);
        doUpload(local, remote);

        DEBUG_EXIT(); 
    }

    void Client::uploadFromMemory(
        std::span<char> local,
        const std::filesystem::path& remote) {

        DEBUG_ENTER();

        std::stringstream message;
        message << "Upload: char buffer of size '" << local.size() << "' to "
            << getCompleteRemoteFilePath(remote); 
        Logger::getInstance().log(LogDomain::Info, message.str());

        checkForEmptyPath(remote);
        doUploadFromMemory(local, remote);

        DEBUG_EXIT(); 

    }

    bool Client::existsOnServer(
        const std::filesystem::path& remote) {

        checkForEmptyPath(remote);
        return doExistsOnServer(remote);
    }

    void Client::deleteOnServer(
        const std::filesystem::path& remote) {

        DEBUG_ENTER();

        checkForEmptyPath(remote);
        doDeleteOnServer(remote);

        DEBUG_EXIT();
    }

    void Client::setRemoteRootPath(
        const std::filesystem::path& remoteRoot) {
        
        this->remoteRoot = remoteRoot;
    }

    std::filesystem::path Client::getCompleteRemoteFilePath(
        const std::filesystem::path& remoteFile) const {

        using namespace std::filesystem;
        return remoteRoot.string() + path::preferred_separator + remoteFile.string();

    }

    void Client::checkForEmptyPath(const std::filesystem::path& path) {
        if (path.string().empty()) {
            throw FileSyncException("Filesystem path is empty",
                __FILE__, __LINE__);
        }
    }

}