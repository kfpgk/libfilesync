#ifndef LIBFILESYNC_CURL_PROTOCOL_CLIENT_HPP
#define LIBFILESYNC_CURL_PROTOCOL_CLIENT_HPP

#include <libfilesync/curl/option/Factory.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>
#include <libfilesync/curl/wrapper/Url.hpp>
#include <libfilesync/curl/storage/FileStorage.hpp>

#include <curl/curl.h>

#include <string>
#include <filesystem>
#include <memory>
#include <string>

namespace filesync::curl {

    namespace unit_test {
        class ProtocolClientTest;
    }
    
    /**
     * Non-virtual interface class for cURL protocols
     * 
     * Roles:
     *  - Service/adaptee of the adapter pattern
     */
    class ProtocolClient {

        public:
            ProtocolClient();
            explicit ProtocolClient(std::unique_ptr<wrapper::Easy> interface);
            virtual ~ProtocolClient() = default;

            [[nodiscard]] bool remoteEntryExists() const;

            void setInterface(std::unique_ptr<wrapper::Easy> interface);
            void setRemoteFile(const std::filesystem::path& path);
            void setLocalFileForUpload(const std::filesystem::path& path);
            void setRemoteDir(const std::filesystem::path& path);
            /**
             * @brief Creates an empty local file to which a download
             * will be written.
             */
            void createLocalFileForDownload(const std::filesystem::path& path);
            void setCreateMissingDirs(bool value); 

            void upload();
            void download();   
            void deleteRemoteFile();
            void createRemoteDir(); 
            void deleteRemoteDir();
    
        protected:
            std::unique_ptr<wrapper::Easy> interface;
            option::Factory optionFactory;
            wrapper::Url coreUrl;
            wrapper::Url activeUrl;

            [[nodiscard]] std::string getRemoteFilePath() const;
            [[nodiscard]] std::string getRemoteDirPath() const;

            /**
             * @brief Discard received data instead of
             * writing to file or stdout.
             */
            void silentOutput();

        private:
            std::filesystem::path remoteFilePath;
            std::filesystem::path remoteDirPath;

            std::unique_ptr<storage::FileStorage> downloadFileStorage;
            std::unique_ptr<storage::FileStorage> uploadFileStorage;

            void validateLocalDownloadFile() const;
            void validateLocalUploadFile() const;
            void validateRemoteFilePath() const;
            void validateRemoteDirPath() const;

            virtual void doUpload();
            virtual void doDownload();
            virtual bool doRemoteEntryExists() const;

            virtual void doCreateRemoteDir() = 0;
            virtual void doDeleteRemoteFile() = 0;
            virtual void doSetCreateMissingDirs(bool value) = 0;  
            virtual void doDeleteRemoteDir() = 0;        

            friend class unit_test::ProtocolClientTest;

    };

}

#endif