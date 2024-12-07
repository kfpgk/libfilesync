#ifndef LIBFILESYNC_TESTS_CURL_FTP_UPLOAD_DOWNLOAD_HPP
#define LIBFILESYNC_TESTS_CURL_FTP_UPLOAD_DOWNLOAD_HPP

#include <tests/template/IntegrationTest.hpp>

#include <string>

namespace filesync::integration_test::curl::ftp {

    class UploadDownload : public IntegrationTest {

        public:
            explicit UploadDownload(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            bool exceptionThrown;
            std::string server;
            std::string pathOnServer;
            std::string file1Name;
            std::string file1Content;
            std::string file1RemotePath;
            std::string file2Name;
            std::string file2Content;
            std::string file2RemotePath;
            std::string binaryFile1Name;
            std::string binaryFile1RemotePath;
            int binaryFile1Content;
            std::string binaryFile2Name;
            
            void setup() override;
            void cleanUp() override; 

            /**
             * @brief Test upload() without defining local and 
             * remote file paths.
             */
            void performUploadUninitialized();
            void evaluateUploadUninitialized();

            /**
             * @brief Test upload() with non existing
             * remote file during initialization
             */
            void performUploadInitNonExistingLocal();
            void evaluateUploadInitNonExistingLocal();

            /**
             * @brief Test upload() with non existing
             * remote file during call to upload()
             */
            void performUploadNonExistingLocal();
            void evaluateUploadNonExistingLocal();

            /**
             * @brief Test upload() with local file
             * initialized for upload
             */
            void performUploadInitializedForDownload();
            void evaluateUploadInitializedForDownload();

            /**
             * @brief Test download() without defining local and 
             * remote file paths.
             */
            void performDownloadUninitialized();
            void evaluateDownloadUninitialized();

            /**
             * @brief Test download() with non existing
             * remote file
             */
            void performDownloadNonExistingRemote();
            void evaluateDownloadNonExistingRemote();

            /**
             * @brief Test download() with undefined
             * remote path
             */
            void performDownloadUndefinedRemote();
            void evaluateDownloadUndefinedRemote();

            /**
             * @brief Test download() with local file
             * initialized for upload
             */
            void performDownloadInitializedForUpload();
            void evaluateDownloadInitializedForUpload();

            /**
             * @brief upload local file to server
             */
            void performUpload();
            void evaluateUpload();

            /**
             * @brief download remote file to
             * local file path
             */
            void performDownload();
            void evaluateDownload();

            /**
             * @brief upload local binary file to server
             */
            void performUploadBinaryFile();
            void evaluateUploadBinaryFile();

            /**
             * @brief download remote binary file to
             * local file path
             */
            void performDownloadBinaryFile();
            void evaluateDownloadBinaryFile();

    };

}

#endif