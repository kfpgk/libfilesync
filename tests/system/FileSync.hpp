#ifndef LIBFILESYNC_TESTS_SYSTEM_FILE_SYNC_HPP
#define LIBFILESYNC_TESTS_SYSTEM_FILE_SYNC_HPP

#include <tests/template/IntegrationTest.hpp>

namespace filesync::integrationtest::system {

    class FileSync : public IntegrationTest {

        public:
            FileSync(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            std::string server;
            std::string pathOnServer;

            void setup() override;
            void perform() override;
            void evaluate() override;
            void cleanUp() override;

            void prepareFileSystem();

    };
    
}

#endif