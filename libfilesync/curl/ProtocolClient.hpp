#ifndef LIBFILESYNC_CURL_PROTOCOL_CLIENT_HPP
#define LIBFILESYNC_CURL_PROTOCOL_CLIENT_HPP

#include <libfilesync/curl/option/Factory.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>
#include <libfilesync/curl/wrapper/Url.hpp>
#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>
#include <libfilesync/curl/storage/MemoryStorageHandle.hpp>

#include <curl/curl.h>

#include <string>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace filesync::curl {

    namespace unit_test {
        class ProtocolClientTest;
    }
    
    /**
     * Non-virtual interface class for cURL protocols
     * 
     * Patterns:
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
            void setInMemoryDataForUpload(const std::span<char>& data);
            void setRemoteDir(const std::filesystem::path& path);
            /**
             * @brief Creates an empty local file to which a download
             * will be written.
             */
            void createLocalFileForDownload(const std::filesystem::path& path);
            void prepareDownloadToMemory();
            /**
             * @brief Returns a read reference to volatile memory in 
             * the form of a std::span
             * 
             * Memory is deallocated when protocol client instance gets
             * destroyed. If you want to have ownership refer to
             * 'getCopyOfDownloadedMemory' or 'takeDownloadedMemory'.
             */
            std::span<char> getReferenceToDownloadedMemory();
            /**
             * @brief Returns a copy of the downloaded memory
             * 
             * Copies the downloaded memory into a std::vector.
             */
            std::vector<char> getCopyOfDownloadedMemory();
            /**
             * @brief Returns a handle which has ownership to the
             * downloaded memory. 
             * 
             * This handle can be passed back to a protocol instance
             * for upload. Involves no copy and takes ownership over
             * the memory away from this protocol instance.
             */
            std::unique_ptr<storage::MemoryStorageHandle> takeDownloadedMemory();
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
            std::unique_ptr<storage::MemoryStorage> downloadMemoryStorage;
            std::unique_ptr<storage::MemoryStorage> uploadMemoryStorage;

            void validateLocalDownloadDestination() const;
            void validateLocalUploadSource() const;
            void validateRemoteFilePath() const;
            void validateRemoteDirPath() const;
            void validateDownloadMemoryStorage() const;

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