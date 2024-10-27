#ifndef LIBFILESYNC_TESTS_PROTOCOL_FTP_UPLOAD_DOWNLOAD_DIR_HPP
#define LIBFILESYNC_TESTS_PROTOCOL_FTP_UPLOAD_DOWNLOAD_DIR_HPP

#include <tests/protocol/ftp/ServerAccess.hpp>

#include <string>

namespace filesync::integrationtest::protocol::ftp {

    /**
     * @brief Upload and download directories via FTP
     * 
     * This integration test requires a FTP server
     * which is configured for anonymous access
     * and allows creating new directories.
     */
    class UploadDownloadDir : public ServerAccess {

        public:        
            UploadDownloadDir(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            std::string dirName1;
            std::string dirName2;

            void setup() override;
            void perform() override;
            void evaluate() override;
            void cleanUp() override;

    };

}

#endif