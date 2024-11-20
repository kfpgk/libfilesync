#ifndef LIBFILESYNC_CURL_PROTOCOL_CLIENT_HPP
#define LIBFILESYNC_CURL_PROTOCOL_CLIENT_HPP

#include <libfilesync/curl/option/Factory.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>
#include <libfilesync/curl/wrapper/Url.hpp>

#include <curl/curl.h>

#include <string>
#include <filesystem>
#include <memory>
#include <string>

namespace filesync::curl {

    namespace unittest {
        class ProtocolClient;
    }
    
    /**
     * Non-virtual interface class for cURL protocols
     * 
     * Roles:
     *  - Service/adaptee of the adapter pattern
     */
    class ProtocolClient {

        enum class FileAccessType {read, write}; 

        public:
            ProtocolClient();
            explicit ProtocolClient(std::unique_ptr<wrapper::Easy> curlEasy);
            virtual ~ProtocolClient();

            [[nodiscard]] bool remoteEntryExists() const;

            void setInterface(std::unique_ptr<wrapper::Easy> curlEasy);
            void setRemoteFile(const std::filesystem::path& path);
            void setLocalFileForUpload(const std::filesystem::path& path);
            void setRemoteDir(const std::filesystem::path& path);
            /**
             * @brief Creates an empty local file to which a download
             * will be written.
             */
            void createLocalFileForDownload(const std::filesystem::path& path);
            void setCreateMissingDirs(bool value);

            void resetPaths(); 

            void upload();
            void download();   
            void deleteRemoteFile();
            void createRemoteDir(); 
            void deleteRemoteDir();
    
        protected:
            std::unique_ptr<wrapper::Easy> curlEasy;
            option::Factory optionFactory;
            wrapper::Url coreUrl;
            wrapper::Url activeUrl;

            [[nodiscard]] std::string getLocalDownloadFilePath() const;
            [[nodiscard]] std::string getLocalUploadFilePath() const;
            [[nodiscard]] std::string getRemoteFilePath() const;
            [[nodiscard]] std::string getRemoteDirPath() const;

            /**
             * @brief Discard received data instead of
             * writing to file or stdout.
             */
            void silentOutput();

        private:
            std::filesystem::path localDownloadFilePath;
            std::filesystem::path localUploadFilePath;
            std::filesystem::path remoteFilePath;
            std::filesystem::path remoteDirPath;
            std::FILE* filePointer = nullptr;

            [[nodiscard]]  std::FILE* getFilePointer(bool throwIfNull = true) const;

            void setFilePointer(const std::filesystem::path& path,
                FileAccessType fileAccess);
            void setInputFileSizeFromFile(const std::filesystem::path& path);
            void setInputFileSize(std::uintmax_t size);
            void setLocalFile(const std::filesystem::path& path,
                FileAccessType fileAccess);

            void validateLocalDownloadFilePath() const;
            void validateLocalUploadFilePath() const;
            void validateRemoteFilePath() const;
            void validateRemoteDirPath() const;

            virtual void doUpload();
            virtual void doDownload();
            virtual bool doRemoteEntryExists() const;

            virtual void doCreateRemoteDir() = 0;
            virtual void doDeleteRemoteFile() = 0;
            virtual void doSetCreateMissingDirs(bool value) = 0;  
            virtual void doDeleteRemoteDir() = 0;        

            friend class filesync::curl::unittest::ProtocolClient;

    };

}

#endif