#ifndef LIBFILESYNC_PROTOCOL_FTP_CLIENT_HPP
#define LIBFILESYNC_PROTOCOL_FTP_CLIENT_HPP

#include <libfilesync/protocol/Client.hpp>
#include <libfilesync/protocol/memory/Handle.hpp>
#include <libfilesync/curl/FtpClient.hpp>
#include <libfilesync/data/Data.hpp>

#include <filesystem>
#include <span>
#include <string>
namespace filesync::protocol {

    /**
     * FTP adapter class
     * 
     * Patterns:
     * 
     *  - Adapter
     */
    class FtpClient : public Client {

        public:
            explicit FtpClient(const std::string& serverAddress);
            FtpClient(const std::string& serverAddress,
                const std::string& remoteRoot);

        private:
            curl::FtpClient curlClient;

            void doUploadFile(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);
            void doUploadDirectory(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);
            void doDownloadFile(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);
            [[nodiscard]] std::unique_ptr<memory::Handle<char>> doDownloadFileToMemory(
                const std::filesystem::path& remote);
            void doDownloadDirectory(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);

            filesync::data::EntryType getRemoteEntryType(
                const std::filesystem::path& remote);   

            std::string doGetProtocolUrlPrefix() const override;

            void doDownload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) override;
            void doDownload(
                std::unique_ptr<memory::Handle<char>>& local,
                const std::filesystem::path& remote) override;
            void doUpload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) override;
            void doUploadFromMemory(
                std::span<char> local,
                const std::filesystem::path& remote) override;
            [[nodiscard]] bool doExistsOnServer(
                const std::filesystem::path& remote) override;
            void doDeleteOnServer(
                const std::filesystem::path& remote) override;

    };

}

#endif