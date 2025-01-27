#ifndef LIBFILESYNC_TESTS_CORE_UNBUFFERED_SYNC_BUFFERED_SYNC_HPP
#define LIBFILESYNC_TESTS_CORE_UNBUFFERED_SYNC_BUFFERED_SYNC_HPP

#include <tests/template/IntegrationTest.hpp>

#include <string>

namespace filesync::integration_test::core::buffered_sync {

    class BufferedSync : public IntegrationTest {

        public:
            BufferedSync(const std::string& name,
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
             * @brief Test initial sync cycle with only
             * a local but no remote file
             */
            void performInitialSyncWithLocalOnly();
            void evaluateInitialSyncWithLocalOnly();

            /**
             * @brief Test initial sync cycle with only
             * a remote but no local file
             */
            void performInitialSyncWithRemoteOnly();
            void evaluateInitialSyncWithRemoteOnly();

            /**
             * @brief Test initial sync cycle with both
             * a remote and a local file
             */
            void performInitialSyncWithExistingLocalAndRemote();
            void evaluateInitialSyncWithExistingLocalAndRemote();

            /**
             * @brief Test syncing with no local changes but changed
             * remote file
             */
            void performSyncWithUnchangedLocalAndChangedRemoteFile();
            void evaluateSyncWithUnchangedLocalAndChangedRemoteFile();

            /**
             * @brief Test syncing with local and remote changes
             */
            void performSyncWithLocalAndRemoteChanges();
            void evaluateSyncWithLocalAndRemoteChanges();

            /**
             * @brief Test syncing with file buffers
             * with local and remote changes
             */
            void performSyncWithFileBuffers();
            void evaluateSyncWithFileBuffers();

            /**
             * @brief Test syncing with in memory 
             * buffers with local and remote changes
             */
            void performSyncWithMemoryBuffers();
            void evaluateSyncWithMemoryBuffers();

    };

}

#endif