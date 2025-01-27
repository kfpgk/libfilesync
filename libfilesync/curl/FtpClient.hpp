#ifndef LIBFILESYNC_CURL_FTP_CLIENT_HPP
#define LIBFILESYNC_CURL_FTP_CLIENT_HPP

#include <libfilesync/curl/ProtocolClient.hpp>
#include <libfilesync/curl/wrapper/Easy.hpp>

#include <curl/curl.h>

#include <memory>
#include <string>

namespace filesync::curl {

    /**
     * @brief FTP wrapper for cURL
     */
    class FtpClient : public ProtocolClient {

        public:
            explicit FtpClient(const std::string& serverAddress);
            FtpClient(const std::string& serverAddress,
                std::unique_ptr<wrapper::Easy> interface);

        private:
            void doCreateRemoteDir() override;
            void doDeleteRemoteFile() override;
            void doSetCreateMissingDirs(bool value) override;
            void doDeleteRemoteDir() override;
            bool doRemoteEntryExists() const override;

            void init(const std::string& serverAddress);

    };

}

#endif