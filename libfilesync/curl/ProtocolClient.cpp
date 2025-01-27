#include <libfilesync/curl/ProtocolClient.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/option/Nobody.hpp>
#include <libfilesync/curl/option/Upload.hpp>
#include <libfilesync/curl/utility/Debug.hpp>

#include <curl/curl.h>

#include <cstdio>
#include <sstream>
#include <vector>

namespace filesync::curl {

    ProtocolClient::ProtocolClient(std::unique_ptr<parser::Nobody> nobodyParser) :
        ProtocolClient{
            std::make_unique<wrapper::Easy>(),
            std::move(nobodyParser)} {

    }

    ProtocolClient::ProtocolClient(
        std::unique_ptr<wrapper::Easy> interface,
        std::unique_ptr<parser::Nobody> nobodyParser) :
            interface{std::move(interface)},
            optionFactory{*this->interface},
            nobodyParser{std::move(nobodyParser)} {

        if (!this->nobodyParser) {
            throw Exception("Invalid parser for 'Nobody' output",
                __FILE__, __LINE__);            
        }

        std::unique_ptr<option::Collection> options = optionFactory.createCollection();

        options->add(optionFactory.createGeneric(CURLOPT_BUFFERSIZE, 102400L));
        options->add(optionFactory.createGeneric(CURLOPT_NOPROGRESS, 1L));
        
        options->add(optionFactory.createGeneric(CURLOPT_TCP_KEEPALIVE, 1L));

        options->set();
    }

    void ProtocolClient::setInterface(
        std::unique_ptr<wrapper::Easy> interface) {

        this->interface = std::move(interface);
    }

    void ProtocolClient::setCreateMissingDirs(bool value) {
        doSetCreateMissingDirs(value);
    }

    void ProtocolClient::setRemoteFile(const std::filesystem::path& path) {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Setting file '" + path.string() + "' as remote file.");
        activeUrl.setPath(path.string());
        activeUrl.validate();
        std::unique_ptr<option::Option> option = 
            optionFactory.createGeneric(CURLOPT_URL, activeUrl.getUrl());
        option->set();
        remoteFilePath = path;
    }
    
    void ProtocolClient::setLocalFileForUpload(const std::filesystem::path& path) {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Setting file '" + path.string() + "' for upload.");
        if (!std::filesystem::is_regular_file(path)) {
            throw Exception(std::string("Local file not found: '" \
                + path.string() + "'"), __FILE__, __LINE__);  
        }
        uploadFileStorage = std::make_unique<storage::FileStorage>(path);
        uploadFileStorage->setupRead(optionFactory);
    }

    void ProtocolClient::setInMemoryDataForUpload(std::span<char> data) {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Setting up memory storage for upload.");
        uploadMemoryStorage = std::make_unique<storage::MemoryStorage>(data);
        uploadMemoryStorage->setupRead(optionFactory);        
    }

    void ProtocolClient::setRemoteDir(const std::filesystem::path& path) {        
        using namespace std::filesystem;
        remoteDirPath = path;
        if (path.string().back() != path::preferred_separator) {
            remoteDirPath += path::preferred_separator;
        }
        LIBFILESYNC_CURL_UTILITY_DEBUG(
            "Setting directory '" + remoteDirPath.string() + "' as remote directory.");
        activeUrl.setPath(remoteDirPath.string());
        activeUrl.validate();
        std::unique_ptr<option::Option> option = 
            optionFactory.createGeneric(CURLOPT_URL, activeUrl.getUrl());
        option->set();      
    }

    void ProtocolClient::createLocalFileForDownload(const std::filesystem::path& path) {
        LIBFILESYNC_CURL_UTILITY_DEBUG(
            "Setting file '" + path.string() + "' as download destination.");
        downloadFileStorage = std::make_unique<storage::FileStorage>(path);
        downloadFileStorage->setupWrite(optionFactory);
    }

    void ProtocolClient::prepareDownloadToMemory() {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Setting up memory storage as download destination");
        downloadMemoryStorage = std::make_unique<storage::MemoryStorage>();
        downloadMemoryStorage->setupWrite(optionFactory);        
    }

    void ProtocolClient::prepareDownloadToMemory(std::size_t bufferSize) {
        LIBFILESYNC_CURL_UTILITY_DEBUG("Setting up memory storage of initial size "\
            << bufferSize << "B as download destination");
        downloadMemoryStorage = std::make_unique<storage::MemoryStorage>(bufferSize);
        downloadMemoryStorage->setupWrite(optionFactory);        
    }

    std::vector<char> ProtocolClient::getDownloadAsCharVector() const {
        validateDownloadMemoryStorage();
        return std::vector<char> {
            downloadMemoryStorage->getDataReference().begin(),
            downloadMemoryStorage->getDataReference().end()};
    }

    std::string ProtocolClient::getDownloadAsString() const {
        validateDownloadMemoryStorage();
        return std::string {
            downloadMemoryStorage->getDataReference().begin(),
            downloadMemoryStorage->getDataReference().end()};
    }

    std::unique_ptr<storage::MemoryStorageHandle> ProtocolClient::takeDownloadMemory() {
        validateDownloadMemoryStorage();
        return std::make_unique<storage::MemoryStorageHandle>(
            storage::MemoryStorageHandle::ConstructorPermission{0},
            std::move(downloadMemoryStorage));
    }

    void ProtocolClient::upload() {
        LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER();

        validateLocalUploadSource();
        validateRemoteFilePath();
        doUpload();

        LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT();
    }

    void ProtocolClient::doUpload() {
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileUpload(true);
            option->set();
            interface->perform();
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    void ProtocolClient::download() {
        LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER();

        validateLocalDownloadDestination();
        validateRemoteFilePath();
        doDownload();

        LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT();
    }

    void ProtocolClient::doDownload() {
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileUpload(false);
            option->set();
            interface->perform();
            if (downloadFileStorage) {
                downloadFileStorage->flush();
            }           
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    void ProtocolClient::deleteRemoteFile() {
        LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER();

        validateRemoteFilePath();
        doDeleteRemoteFile();

        LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT();       
    }

    bool ProtocolClient::remoteEntryExists() const {
        return doRemoteEntryExists();
    }

    bool ProtocolClient::doRemoteEntryExists() const {
        try {
            std::unique_ptr<option::Collection> options = optionFactory.createCollection();
            
            options->add(optionFactory.createVolatileNobody());
            options->add(optionFactory.createGeneric(CURLOPT_WRITEDATA, nullptr));
            options->set();
            interface->perform();
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

    std::size_t ProtocolClient::getRemoteFileSize() {
        LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER();
   
        std::size_t fileSize = doGetRemoteFileSize();

        LIBFILESYNC_CURL_UTILITY_DEBUG(
            "Remote file size is: " + std::to_string(fileSize) + "B");

        LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT();

        return fileSize;       
    }

    std::size_t ProtocolClient::doGetRemoteFileSize() {
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileNobody();
            option->set();
            prepareDownloadToMemory();
            interface->perform();
            nobodyParser->parse(getDownloadAsString());
            return nobodyParser->getFileSize();    
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }       
    }

    void ProtocolClient::createRemoteDir() {
        LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER();

        validateRemoteDirPath();    
        doCreateRemoteDir();

        LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT();  
    }

    void ProtocolClient::deleteRemoteDir() {
        LIBFILESYNC_CURL_UTILITY_DEBUG_ENTER();

        validateRemoteDirPath();    
        doDeleteRemoteDir();

        LIBFILESYNC_CURL_UTILITY_DEBUG_EXIT();  
    }

    void ProtocolClient::silentOutput() {
        std::unique_ptr<option::Option> option = 
            optionFactory.createGeneric(CURLOPT_WRITEDATA, nullptr);
        option->set();
    }

    std::string ProtocolClient::getRemoteFilePath() const {
        return remoteFilePath.string();
    }

    std::string ProtocolClient::getRemoteDirPath() const {
        return remoteDirPath.string();
    }

    void ProtocolClient::validateLocalDownloadDestination() const {
        if (!downloadFileStorage && !downloadMemoryStorage) {
            throw Exception("Local download storage not set up.", \
                __FILE__, __LINE__); 
        }
    }

    void ProtocolClient::validateLocalUploadSource() const {
        if (!uploadFileStorage && !uploadMemoryStorage) {
            throw Exception("Local upload storage not set up.", \
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

    void ProtocolClient::validateDownloadMemoryStorage() const {
        if (!downloadMemoryStorage) {
            throw Exception("Download memory storage is empty",
                __FILE__, __LINE__);
        }
    }

}