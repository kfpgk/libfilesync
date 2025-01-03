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

    void Client::download(
        HandleOrFilePath local,
        const std::filesystem::path& remote) {

        DEBUG_ENTER(); 

        if (std::holds_alternative<std::reference_wrapper<const std::filesystem::path>>(local)) {
            download(std::get<
                std::reference_wrapper<const std::filesystem::path>>(local).get(), remote);
        } else if (std::holds_alternative<
            std::reference_wrapper<std::unique_ptr<memory::Handle<char>>>>(local)) {

            download(std::get<
                std::reference_wrapper<std::unique_ptr<memory::Handle<char>>>>(local).get(), remote);
        } else {
            throw FileSyncException("std::variant holds unexpected value.",
                __FILE__, __LINE__);
        }

        DEBUG_EXIT();

    }

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
        const char* local,
        const std::filesystem::path& remote) {

        upload(std::filesystem::path(local), remote);
        
    }

    void Client::upload(
        const std::string& local,
        const std::filesystem::path& remote) {

        upload(std::filesystem::path(local), remote);
        
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

    void Client::upload(
        std::unique_ptr<memory::Handle<char>>& local,
        const std::filesystem::path& remote) {

        if (!local) {
            throw FileSyncException("Memory handle is empty. Cannot upload",
                __FILE__, __LINE__);            
        }
        upload(local->data(), remote);

    }

    void Client::upload(
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