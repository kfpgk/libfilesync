#ifndef LIBFILESYNC_PROTOCOL_FTP_CLIENT_HPP
#define LIBFILESYNC_PROTOCOL_FTP_CLIENT_HPP

#include <libfilesync/protocol/Client.hpp>
#include <libfilesync/curl/FtpClient.hpp>
#include <libfilesync/data/Data.hpp>

#include <string>
#include <filesystem>

namespace filesync::protocol {

    /**
     * FTP adapter class
     * 
     * Patterns:
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
            void doDownloadDirectory(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);

            data::EntryType getRemoteEntryType(
                const std::filesystem::path& remote);   

            void doDownload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) override;
            void doUpload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) override;
            [[nodiscard]] bool doExistsOnServer(
                const std::filesystem::path& remote) override;
            void doDeleteOnServer(
                const std::filesystem::path& remote) override;

    };

}

#endif