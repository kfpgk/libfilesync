#ifndef LIBFILESYNC_TESTS_PROTOCOL_FTP_SERVER_ACCESS_HPP
#define LIBFILESYNC_TESTS_PROTOCOL_FTP_SERVER_ACCESS_HPP

#include <tests/template/IntegrationTest.hpp>

namespace filesync::integration_test::protocol::ftp {

    /**
     * @brief Abstract integration test class
     * to access a FTP server
     */
    class ServerAccess : public IntegrationTest {

        public:
            ServerAccess(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);           

            void setUploadFileName(const std::string& fileName);
            void setDownloadFileName(const std::string& fileName);
            void setRemoteFileName(const std::string& fileName);

        protected:
            std::string server;
            std::string pathOnServer;
            std::string uploadFileName = "testfile_upload";
            std::string downloadFileName = "testfile_download";
            std::string remoteFileName = "testfile_remote";

        private:
            void setup() override = 0;
            void cleanUp() override = 0;

    };

}

#endif