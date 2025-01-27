#ifndef LIBFILESYNC_TESTS_SYSTEM_ERROR_HANDLING_FILE_SYNC_ERROR_HANDLING_HPP
#define LIBFILESYNC_TESTS_SYSTEM_ERROR_HANDLING_FILE_SYNC_ERROR_HANDLING_HPP

#include <tests/template/IntegrationTest.hpp>

namespace filesync::integration_test::system::error_handling {

    class FileSyncErrorHandling : public IntegrationTest {

        public:
            FileSyncErrorHandling(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            std::string server;
            std::string pathOnServer;

            bool exceptionThrown = false;

            void setup() override;

            void performMissingSyncContent();
            void evaluateMissingSyncContent();
            
            void prepareFileSystem();

    };
    
}

#endif