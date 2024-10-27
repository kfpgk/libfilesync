#include <libfilesync/curl/ProtocolClient.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/option/Nobody.hpp>
#include <libfilesync/curl/option/Upload.hpp>
#include <libfilesync/utility/Debug.hpp>
#include <libfilesync/utility/Literals.hpp>
#include <libfilesync/curl/interface/Easy.hpp>

#include <curl/curl.h>

#include <sstream>
#include <cstdio>
#include <cstring>

using namespace filesync::utility;

namespace filesync::curl {

    ProtocolClient::ProtocolClient() :
        ProtocolClient{std::make_unique<interface::Easy>()} {

    }

    ProtocolClient::ProtocolClient(std::unique_ptr<interface::Easy> curlEasy) :
        curlEasy{std::move(curlEasy)},
        optionFactory{*this->curlEasy} {

        std::unique_ptr<option::Collection> options = optionFactory.createCollection();

        options->add(optionFactory.createGeneric(CURLOPT_BUFFERSIZE, 102400L));
        options->add(optionFactory.createGeneric(CURLOPT_NOPROGRESS, 1L));
        
        options->add(optionFactory.createGeneric(CURLOPT_TCP_KEEPALIVE, 1L));

        options->set();
    }

    ProtocolClient::~ProtocolClient() {
        if (filePointer) {
            std::fclose(filePointer);
        }
    }

    void ProtocolClient::resetPaths() {
        localDownloadFilePath.clear();
        localUploadFilePath.clear();
        remoteFilePath.clear();
        remoteDirPath.clear();
        filePointer = nullptr;        
    }

    void ProtocolClient::setInterface(
        std::unique_ptr<interface::Easy> curlEasy) {

        this->curlEasy = std::move(curlEasy);
    }

    void ProtocolClient::setCreateMissingDirs(bool value) {
        doSetCreateMissingDirs(value);
    }

    void ProtocolClient::setRemoteFile(const std::filesystem::path& path) {
        DEBUG("Setting file '" + path.string() + "' as remote file.");
        activeUrl.setPath(path.string());
        activeUrl.validate();
        std::unique_ptr<option::Option> option = 
            optionFactory.createGeneric(CURLOPT_URL, activeUrl.getUrl());
        option->set();
        remoteFilePath = path;
    }
    
    void ProtocolClient::setLocalFileForUpload(const std::filesystem::path& path) {
        DEBUG("Setting file '" + path.string() + "' for upload.");
        if (!std::filesystem::is_regular_file(path)) {
            throw Exception(std::string("Local file not found: '" \
                + path.string() + "'"), \
                __FILE__, __LINE__);  
        }
        setLocalFile(path, FileAccessType::read);
        setInputFileSizeFromFile(path);
        localUploadFilePath = path;
    }

    void ProtocolClient::setRemoteDir(const std::filesystem::path& path) {        
        using namespace std::filesystem;
        remoteDirPath = path;
        if (path.string().back() != path::preferred_separator) {
            remoteDirPath += path::preferred_separator;
        }
        DEBUG("Setting directory '" + remoteDirPath.string() + "' as remote directory.");
        activeUrl.setPath(remoteDirPath.string());
        activeUrl.validate();
        std::unique_ptr<option::Option> option = 
            optionFactory.createGeneric(CURLOPT_URL, activeUrl.getUrl());
        option->set();      
    }

    void ProtocolClient::createLocalFileForDownload(const std::filesystem::path& path) {
        DEBUG("Setting file '" + path.string() + "' as download destination.");
        setLocalFile(path, FileAccessType::write);
        localDownloadFilePath = path;
    }

    void ProtocolClient::setLocalFile(const std::filesystem::path& path,
        FileAccessType fileAccess) {

        setFilePointer(path, fileAccess);
        std::unique_ptr<option::Option> option;
        if (fileAccess == FileAccessType::read) {
            option = optionFactory.createGeneric(CURLOPT_READDATA, getFilePointer());
        } else if(fileAccess == FileAccessType::write) {
            option = optionFactory.createGeneric(CURLOPT_WRITEDATA, getFilePointer());
        }
        option->set();
    }

    void ProtocolClient::upload() {
        DEBUG_ENTER();

        validateLocalUploadFilePath();
        validateRemoteFilePath();
        doUpload();

        DEBUG_EXIT();
    }

    void ProtocolClient::doUpload() {
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileUpload(true);
            option->set();
            curlEasy->run();
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    void ProtocolClient::download() {
        DEBUG_ENTER();

        validateLocalDownloadFilePath();
        validateRemoteFilePath();
        doDownload();

        DEBUG_EXIT();
    }

    void ProtocolClient::doDownload() {
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileUpload(false);
            option->set();
            curlEasy->run();
            if (getFilePointer(false)) {
                std::fflush(getFilePointer());
            }           
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    void ProtocolClient::deleteRemoteFile() {
        DEBUG_ENTER();

        validateRemoteFilePath();
        doDeleteRemoteFile();

        DEBUG_EXIT();       
    }

    bool ProtocolClient::remoteEntryExists() const {
        return doRemoteEntryExists();
    }

    bool ProtocolClient::doRemoteEntryExists() const {
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileNobody();
            option->set();
            curlEasy->run();
            return true;
        } catch(Exception& e) {
            if (e.getCurlCode() == CURLE_REMOTE_FILE_NOT_FOUND) {
                return false;
            } else {
                e.addContext(__FILE__, __LINE__);
                throw e;
            }
        }       
    }

    void ProtocolClient::createRemoteDir() {
        DEBUG_ENTER();

        validateRemoteDirPath();    
        doCreateRemoteDir();

        DEBUG_EXIT();  
    }

    void ProtocolClient::deleteRemoteDir() {
        DEBUG_ENTER();

        validateRemoteDirPath();    
        doDeleteRemoteDir();

        DEBUG_EXIT();  
    }

    void ProtocolClient::silentOutput() {
        std::unique_ptr<option::Option> option = 
            optionFactory.createGeneric(CURLOPT_WRITEDATA, nullptr);
        option->set();
    }

    std::FILE* ProtocolClient::getFilePointer(bool throwIfNull) const {
        if (!filePointer && throwIfNull) {
            throw Exception("filePointer not set '", \
                __FILE__, __LINE__);               
        }
        return filePointer;
    }

    void ProtocolClient::setFilePointer(const std::filesystem::path& path,
        FileAccessType fileAccess) {

        std::string fileAccessFlags = "r+b";
        if (fileAccess == FileAccessType::write) {
            fileAccessFlags = "w+b";
        }    
        if (filePointer) {
            std::fclose(filePointer);
        }
        filePointer = nullptr;
        filePointer = std::fopen(path.string().c_str(), fileAccessFlags.c_str());
        if (!filePointer) {
            throw Exception(std::string("fopen() failed on '" \
                + path.string() + "' with '" + std::strerror(errno) \
                + "'"), __FILE__, __LINE__);              
        }
    }

    void ProtocolClient::setInputFileSizeFromFile(const std::filesystem::path& path) {
        std::uintmax_t fileSize = std::filesystem::file_size(path);
        if (fileSize < 0) {
            throw Exception(std::string("could not get file size of '" \
                + path.string()), __FILE__, __LINE__);  
        }
        setInputFileSize(fileSize);
    }

    void ProtocolClient::setInputFileSize(std::uintmax_t size) {
        try {
            using namespace utility::literals;
            std::unique_ptr<option::Option> option;
            if (size > 2_GB) {
                option = optionFactory.createGeneric(CURLOPT_INFILESIZE_LARGE, size);
            } else {
                option = optionFactory.createGeneric(CURLOPT_INFILESIZE, size);
            }
            option->set();
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    std::string ProtocolClient::getLocalDownloadFilePath() const {
        return localDownloadFilePath.string();
    }

    std::string ProtocolClient::getLocalUploadFilePath() const {
        return localUploadFilePath.string();
    }

    std::string ProtocolClient::getRemoteFilePath() const {
        return remoteFilePath.string();
    }

    std::string ProtocolClient::getRemoteDirPath() const {
        return remoteDirPath.string();
    }

    void ProtocolClient::validateLocalDownloadFilePath() const {
        if (getLocalDownloadFilePath().empty()) {
            throw Exception("Local download file path not set.", \
                __FILE__, __LINE__); 
        }
    }

    void ProtocolClient::validateLocalUploadFilePath() const {
        if (getLocalUploadFilePath().empty()) {
            throw Exception("Local upload file path not set.", \
                __FILE__, __LINE__); 
        }
    }

    void ProtocolClient::validateRemoteFilePath() const {
        if (getRemoteFilePath().empty()) {
            throw Exception("Remote file path not set.", \
                __FILE__, __LINE__); 
        }
    }

    void ProtocolClient::validateRemoteDirPath() const {
        if (getRemoteDirPath().empty()) {
            throw Exception("Remote directory path not set.", \
                __FILE__, __LINE__); 
        }
    }

}