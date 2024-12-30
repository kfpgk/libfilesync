#include <libfilesync/protocol/FtpClient.hpp>
#include <libfilesync/protocol/memory/CurlHandle.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/utility/Debug.hpp>

namespace filesync::protocol {

    FtpClient::FtpClient(const std::string& serverAddress) :
        curlClient(serverAddress) {

    }

    FtpClient::FtpClient(const std::string& serverAddress,
        const std::string& remoteRoot) :
        curlClient(serverAddress),
        Client(remoteRoot) {

    }

    void FtpClient::doDownload(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        switch(getRemoteEntryType(remote)) {
            case filesync::data::EntryType::dir:
                doDownloadDirectory(local, remote);
                break;
            case filesync::data::EntryType::file:
                doDownloadFile(local, remote);
                break;
            default:
                throw FileSyncException("Remote entry '" 
                    + getCompleteRemoteFilePath(remote).string()  + "' not found.");
        }
               
    }

    void FtpClient::doDownloadFile(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        try {
            curlClient.createLocalFileForDownload(local);
            curlClient.setRemoteFile(getCompleteRemoteFilePath(remote));
            curlClient.download();
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    void FtpClient::doDownloadDirectory(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        try {
            curlClient.setRemoteDir(getCompleteRemoteFilePath(remote));
            if (!curlClient.remoteEntryExists()) {
                throw FileSyncException("Remote directory '" + getCompleteRemoteFilePath(remote).string()  + "' not found.");                
            }
            if (!std::filesystem::is_directory(local)) {
                std::filesystem::create_directory(local);
            }
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }        
    }

    void FtpClient::doDownload(
        std::unique_ptr<memory::Handle<char>>& local,
        const std::filesystem::path& remote) {

        switch(getRemoteEntryType(remote)) {
            case filesync::data::EntryType::dir:
                throw FileSyncException("Cannot download remote directory to "\
                    "memory buffer");
                break;
            case filesync::data::EntryType::file:
                local = doDownloadFileToMemory(remote);
                break;
            default:
                throw FileSyncException("Remote entry '" 
                    + getCompleteRemoteFilePath(remote).string()  + "' not found.");
        }
               
    }

    std::unique_ptr<memory::Handle<char>> FtpClient::doDownloadFileToMemory(
        const std::filesystem::path& remote) {

        try {
            curlClient.prepareDownloadToMemory();
            curlClient.setRemoteFile(getCompleteRemoteFilePath(remote));
            curlClient.download();
            return std::make_unique<memory::CurlHandle>(curlClient.takeDownloadMemory());
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }


    }

    void FtpClient::doUpload(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        if (std::filesystem::is_directory(local)) {
            doUploadDirectory(local, remote);
        } else if (std::filesystem::is_regular_file(local)) {
            doUploadFile(local, remote);
        }
    }

    void FtpClient::doUploadFile(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        try {
            curlClient.setLocalFileForUpload(local);
            curlClient.setRemoteFile(getCompleteRemoteFilePath(remote));
            curlClient.upload();
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }

    }

    void FtpClient::doUploadDirectory(
        const std::filesystem::path& local,
        const std::filesystem::path& remote) {

        try {
            if (!std::filesystem::is_directory(local)) {
                throw FileSyncException("Local directory '" + local.string()  + "' not found.");
            }
            curlClient.setRemoteDir(getCompleteRemoteFilePath(remote));
            if (!curlClient.remoteEntryExists()) {
                curlClient.createRemoteDir();
            }           
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }

    }        

    void FtpClient::doUploadFromMemory(
        std::span<char> local,
        const std::filesystem::path& remote) {

        try {
            curlClient.setInMemoryDataForUpload(local);
            curlClient.setRemoteFile(getCompleteRemoteFilePath(remote));
            curlClient.upload();
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }

    }

    bool FtpClient::doExistsOnServer(const std::filesystem::path& remote) {
        try {
            return getRemoteEntryType(remote) != filesync::data::EntryType::none; 
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    void FtpClient::doDeleteOnServer(const std::filesystem::path& remote) {
        
        try {
        switch(getRemoteEntryType(remote)) {
            case filesync::data::EntryType::dir:
                curlClient.setRemoteDir(getCompleteRemoteFilePath(remote));
                curlClient.deleteRemoteDir();
                break;
            case filesync::data::EntryType::file:
                curlClient.setRemoteFile(getCompleteRemoteFilePath(remote));
                curlClient.deleteRemoteFile();
                break;
            default:
                throw FileSyncException("Remote entry '" + getCompleteRemoteFilePath(remote).string()  + "' not found.");
        }     
        } catch (FileSyncException& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    filesync::data::EntryType FtpClient::getRemoteEntryType(
        const std::filesystem::path& remote) {

        curlClient.setRemoteFile(getCompleteRemoteFilePath(remote));
        if (curlClient.remoteEntryExists()) {
            DEBUG("Remote entry '" << getCompleteRemoteFilePath(remote).string() << "' is a file.");
            return filesync::data::EntryType::file;
        }
        curlClient.setRemoteDir(getCompleteRemoteFilePath(remote));
        if (curlClient.remoteEntryExists()) {
            DEBUG("Remote entry '" << getCompleteRemoteFilePath(remote).string() << "' is a directory.");
            return filesync::data::EntryType::dir;
        }
        DEBUG("Remote entry '" << getCompleteRemoteFilePath(remote).string() << "' does not exist.");
        return filesync::data::EntryType::none;
    }

}