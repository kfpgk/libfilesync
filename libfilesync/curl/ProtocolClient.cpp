#include <libfilesync/curl/ProtocolClient.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/option/Nobody.hpp>
#include <libfilesync/curl/option/Upload.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <curl/curl.h>

#include <cstdio>
#include <sstream>

using namespace filesync::utility;

namespace filesync::curl {

    ProtocolClient::ProtocolClient() :
        ProtocolClient{std::make_unique<wrapper::Easy>()} {

    }

    ProtocolClient::ProtocolClient(std::unique_ptr<wrapper::Easy> interface) :
        interface{std::move(interface)},
        optionFactory{*this->interface} {

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
        uploadFileStorage = std::make_unique<storage::FileStorage>(path);
        uploadFileStorage->setupRead(optionFactory);
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
        downloadFileStorage = std::make_unique<storage::FileStorage>(path);
        downloadFileStorage->setupWrite(optionFactory);
    }

    void ProtocolClient::upload() {
        DEBUG_ENTER();

        validateLocalUploadFile();
        validateRemoteFilePath();
        doUpload();

        DEBUG_EXIT();
    }

    void ProtocolClient::doUpload() {
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileUpload(true);
            option->set();
            interface->run();
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
    }

    void ProtocolClient::download() {
        DEBUG_ENTER();

        validateLocalDownloadFile();
        validateRemoteFilePath();
        doDownload();

        DEBUG_EXIT();
    }

    void ProtocolClient::doDownload() {
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileUpload(false);
            option->set();
            interface->run();
            if (downloadFileStorage) {
                downloadFileStorage->flush();
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
            interface->run();
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

    std::string ProtocolClient::getRemoteFilePath() const {
        return remoteFilePath.string();
    }

    std::string ProtocolClient::getRemoteDirPath() const {
        return remoteDirPath.string();
    }

    void ProtocolClient::validateLocalDownloadFile() const {
        if (downloadFileStorage) {
            throw Exception("Local download file storage not set up.", \
                __FILE__, __LINE__); 
        }
    }

    void ProtocolClient::validateLocalUploadFile() const {
        if (uploadFileStorage) {
            throw Exception("Local upload file storage not set up.", \
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