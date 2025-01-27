#ifndef LIBFILESYNC_TESTS_PROTOCOL_FTP_UPLOAD_DOWNLOAD_FILE_HPP
#define LIBFILESYNC_TESTS_PROTOCOL_FTP_UPLOAD_DOWNLOAD_FILE_HPP

#include <tests/protocol/ftp/ServerAccess.hpp>

#include <string>

namespace filesync::integration_test::protocol::ftp {

    class UploadDownloadFile : public ServerAccess {

        public:        
            UploadDownloadFile(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            std::string testFileContent = "Test file content";

            void setup() override;
            void perform() override;
            void evaluate() override;
            void cleanUp() override;

    };

}

#endif