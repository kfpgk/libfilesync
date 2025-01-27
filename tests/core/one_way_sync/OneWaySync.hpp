#ifndef LIBFILESYNC_TESTS_CORE_ONE_WAY_SYNC_ONE_WAY_SYNC_HPP
#define LIBFILESYNC_TESTS_CORE_ONE_WAY_SYNC_ONE_WAY_SYNC_HPP

#include <tests/template/IntegrationTest.hpp>

#include <string>

namespace filesync::integration_test::core::one_way_sync {

    class OneWaySync : public IntegrationTest {

        public:
            OneWaySync(const std::string& name,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            std::string server;
            std::string pathOnServer;

            std::string file1Name;
            std::string file1Content;
            std::string file2Name;
            std::string file2Content;

            /**
             * @brief Sync a file to the server
             */
            void performUploadSync();
            void evaluateUploadSync();

            /**
             * @brief Sync a file to the server and 
             * overwrite existing remote file
             */
            void performUploadOverwrite();
            void evaluateUploadOverwrite();

            /**
             * @brief Perform upload sync with no local
             * but only a remote file
             * 
             * Expected behavior is a download of the
             * remote file.
             */
            void performUploadSyncWithRemoteFileOnly();
            void evaluateUploadSyncWithRemoteFileOnly();

            /**
             * @brief Perform upload sync with neither local
             * nor remote file
             * 
             * Expected behavior is an exception
             */
            void performUploadSyncWithMissingFiles();
            void evaluateUploadSyncWithMissingFiles();

            /**
             * @brief Sync a file from the server
             */
            void performDownloadSync();
            void evaluateDownloadSync();

            /**
             * @brief Sync a file from the server and 
             * overwrite existing local file
             */
            void performDownloadOverwrite();
            void evaluateDownloadOverwrite();

            /**
             * @brief Perform download sync with no remote
             * but only a local file
             * 
             * Expected behavior is an upload of the
             * file file.
             */
            void performDownloadSyncWithLocalFileOnly();
            void evaluateDownloadSyncWithLocalFileOnly();

            /**
             * @brief Perform download sync with neither local
             * nor remote file
             * 
             * Expected behavior is an exception
             */
            void performDownloadSyncWithMissingFiles();
            void evaluateDownloadSyncWithMissingFiles();

    };

}

#endif