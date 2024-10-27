#ifndef LIBFILESYNC_TESTS_DEBUG_DEBUG_HPP
#define LIBFILESYNC_TESTS_DEBUG_DEBUG_HPP

#include <tests/template/IntegrationTest.hpp>

#include <string>

namespace filesync::integrationtest::debug {

    class Debug : public IntegrationTest {

        public:
            Debug(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            std::string server;
            std::string pathOnServer;

            void setup() override;
            void perform() override;
            void evaluate() override;
            void cleanUp() override;

    };

}

#endif