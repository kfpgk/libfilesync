#ifndef LIBFILESYNC_TESTS_PROTOCOL_FTP_PERFORMANCE_UPLOAD_DOWNLOAD_PERFORMANCE_HPP
#define LIBFILESYNC_TESTS_PROTOCOL_FTP_PERFORMANCE_UPLOAD_DOWNLOAD_PERFORMANCE_HPP

#include <tests/protocol/ftp/ServerAccess.hpp>
#include <libfilesync/protocol/memory/Handle.hpp>

#include <chrono>
#include <memory>
#include <string>

namespace filesync::integration_test::protocol::ftp::performance {

    class UploadDownloadPerformance : public ServerAccess {

        public:
            UploadDownloadPerformance(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            std::string testFileContent = "Test file content";
            std::chrono::milliseconds executionTime;
            std::unique_ptr<filesync::protocol::memory::Handle<char>> handle;

            void setup() override;
            void cleanUp() override;

            /**
             * @brief Test performance of file upload
             */
            void performUploadFromFile();
            void evaluateUploadFromFile();

            /**
             * @brief Test performance of file download
             */
            void performDownloadToFile();
            void evaluateDownloadToFile();

            /**
             * @brief Test performance of download to memory
             */
            void performDownloadToMemory();
            void evaluateDownloadToMemory();

            /**
             * @brief Test performance of memory upload
             */
            void performUploadFromMemory();
            void evaluateUploadFromMemory();

    };

}

#endif