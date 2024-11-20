#include <libfilesync/curl/FtpClient.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/curl/wrapper/Url.hpp>
#include <libfilesync/curl/option/Factory.hpp>
#include <libfilesync/curl/option/Generic.hpp>
#include <libfilesync/curl/option/Option.hpp>
#include <libfilesync/curl/option/Collection.hpp>
#include <libfilesync/utility/Debug.hpp>

#define FILESYNC_CURL_URL_FTP_PREFIX "ftp"

namespace filesync::curl {

    FtpClient::FtpClient(const std::string& serverAddress,
        std::unique_ptr<wrapper::Easy> curlEasy) :
            ProtocolClient(std::move(curlEasy)) {
    
        init(serverAddress);
    }

    FtpClient::FtpClient(const std::string& serverAddress) {
        init(serverAddress);
    }

    void FtpClient::init(const std::string& serverAddress) {
        DEBUG_ENTER();
        try {

            std::unique_ptr<option::Collection> options = optionFactory.createCollection();

            options->add(optionFactory.createGeneric(CURLOPT_FTP_SKIP_PASV_IP, 1L));

            activeUrl.setHost(serverAddress);
            activeUrl.setScheme(FILESYNC_CURL_URL_FTP_PREFIX);
            activeUrl.validate();        
            options->add(optionFactory.createGeneric(CURLOPT_URL, activeUrl.getUrl()));
            
            options->add(optionFactory.createGeneric(CURLOPT_WRITEDATA, nullptr));

            options->set();
            curlEasy->run();

            DEBUG("Successfully connected to '" << activeUrl.getUrl() << "'");

        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
        DEBUG_EXIT();   
    }

    void FtpClient::doDeleteRemoteFile() {
        DEBUG_ENTER(); 
        try {
            std::unique_ptr<option::Collection> options = optionFactory.createCollection();
            
            std::unique_ptr<option::Quote> volatileQuote = 
                optionFactory.createVolatileQuote();
            volatileQuote->addCommand("CWD /");
            volatileQuote->addCommand("DELE " + getRemoteFilePath());
            options->add(std::move(volatileQuote));

            wrapper::Url url;
            url.setHost(activeUrl.getHost());
            url.setScheme(activeUrl.getScheme());            
            options->add(optionFactory.createGeneric(CURLOPT_URL, url.getUrl()));

            options->add(optionFactory.createGeneric(CURLOPT_WRITEDATA, nullptr));

            options->set();
            curlEasy->run();      
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }        
        DEBUG_EXIT();       
    }

    void FtpClient::doSetCreateMissingDirs(bool value) {
        std::unique_ptr<option::Option> option;
        if (value) {
            option = optionFactory.createGeneric(CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
        } else {
            option = optionFactory.createGeneric(CURLOPT_FTP_CREATE_MISSING_DIRS, 0L);
        }
        option->set();
    }

    void FtpClient::doCreateRemoteDir() {
        DEBUG_ENTER();
        try {
            std::unique_ptr<option::Collection> options = optionFactory.createCollection();
            
            std::unique_ptr<option::Quote> volatileQuote = 
                optionFactory.createVolatileQuote();
            volatileQuote->addCommand("CWD /");
            volatileQuote->addCommand("MKD " + getRemoteDirPath());
            options->add(std::move(volatileQuote));

            wrapper::Url url;
            url.setHost(activeUrl.getHost());
            url.setScheme(activeUrl.getScheme());      
            options->add(optionFactory.createGeneric(CURLOPT_URL, url.getUrl()));

            options->add(optionFactory.createGeneric(CURLOPT_WRITEDATA, nullptr));

            options->set();
            curlEasy->run();      
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }
        DEBUG_EXIT();            
    }

    void FtpClient::doDeleteRemoteDir() {
        DEBUG_ENTER();
        try {
            std::unique_ptr<option::Collection> options = optionFactory.createCollection();
            
            std::unique_ptr<option::Quote> volatileQuote = 
                optionFactory.createVolatileQuote();
            volatileQuote->addCommand("CWD /");
            volatileQuote->addCommand("RMD " + getRemoteDirPath());
            options->add(std::move(volatileQuote));

            wrapper::Url url;
            url.setHost(activeUrl.getHost());
            url.setScheme(activeUrl.getScheme());            
            options->add(optionFactory.createGeneric(CURLOPT_URL, url.getUrl()));

            options->add(optionFactory.createGeneric(CURLOPT_WRITEDATA, nullptr));

            options->set();
            curlEasy->run();      
        } catch(Exception& e) {
            e.addContext(__FILE__, __LINE__);
            throw e;
        }   
        DEBUG_EXIT();         
    }

    bool FtpClient::doRemoteEntryExists() const {
        DEBUG_ENTER();
        try {
            std::unique_ptr<option::Option> option = 
                optionFactory.createVolatileNobody();
            option->set();
            curlEasy->run();
        } catch(Exception& e) {
            DEBUG_EXIT();
            return false;
        }
        DEBUG_EXIT();
        return true;       
    }

}