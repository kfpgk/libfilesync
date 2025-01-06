#ifndef LIBFILESYNC_CURL_PROTOCOL_CLIENT_HPP
#define LIBFILESYNC_CURL_PROTOCOL_CLIENT_HPP

#include <libfilesync/curl/option/Factory.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>
#include <libfilesync/curl/wrapper/Url.hpp>
#include <libfilesync/curl/storage/FileStorage.hpp>
#include <libfilesync/curl/storage/MemoryStorage.hpp>
#include <libfilesync/curl/storage/MemoryStorageHandle.hpp>
#include <libfilesync/curl/parser/Nobody.hpp>

#include <curl/curl.h>

#include <cstddef>
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
            explicit ProtocolClient(std::unique_ptr<parser::Nobody> nobodyParser);
            ProtocolClient(std::unique_ptr<wrapper::Easy> interface,
                std::unique_ptr<parser::Nobody> nobodyParser);
            virtual ~ProtocolClient() = default;

            [[nodiscard]] bool remoteEntryExists() const;

            void setInterface(std::unique_ptr<wrapper::Easy> interface);
            void setRemoteFile(const std::filesystem::path& path);
            void setLocalFileForUpload(const std::filesystem::path& path);
            void setInMemoryDataForUpload(std::span<char> data);
            void setRemoteDir(const std::filesystem::path& path);
            /**
             * @brief Creates an empty local file to which a download
             * will be written.
             */
            void createLocalFileForDownload(const std::filesystem::path& path);
            /**
             * @brief Prepare a chunk of memory as download destination.
             * 
             * For large downloads this may result in multiple reallocations.
             * If possible use the overload including bufferSize to 
             * reduce number of reallocations.
             */
            void prepareDownloadToMemory();
            /**
             * @brief Prepare a chunk of memory of size `buffersize`
             * as download destination.
             */
            void prepareDownloadToMemory(std::size_t bufferSize);
            /**
             * @brief Returns a copy of the most recently downloaded
             * memory as vector of chars.
             */
            std::vector<char> getDownloadAsCharVector() const;
            /**
             * @brief Returns a copy of the most recently downloaded
             * memory as a string.
             */
            std::string getDownloadAsString() const;
            /**
             * @brief Returns a handle which has ownership to the
             * most recently downloaded memory. 
             * 
             * Involves no copy and takes ownership over
             * the memory away from this protocol instance.
             */
            [[nodiscard]] std::unique_ptr<storage::MemoryStorageHandle> takeDownloadMemory();
            void setCreateMissingDirs(bool value); 

            void upload();
            void download();   
            void deleteRemoteFile();
            void createRemoteDir(); 
            void deleteRemoteDir();
            [[nodiscard]] std::size_t getRemoteFileSize();
    
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

            std::unique_ptr<parser::Nobody> nobodyParser;

            void validateLocalDownloadDestination() const;
            void validateLocalUploadSource() const;
            void validateRemoteFilePath() const;
            void validateRemoteDirPath() const;
            void validateDownloadMemoryStorage() const;

            virtual void doUpload();
            virtual void doDownload();
            virtual bool doRemoteEntryExists() const;
            virtual std::size_t doGetRemoteFileSize();

            virtual void doCreateRemoteDir() = 0;
            virtual void doDeleteRemoteFile() = 0;
            virtual void doSetCreateMissingDirs(bool value) = 0;  
            virtual void doDeleteRemoteDir() = 0;

            friend class unit_test::ProtocolClientTest;

    };

}

#endif