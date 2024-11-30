#include <tests/debug/Debug.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/protocol/FtpClient.hpp>
#include <tests/config/config.h>

#include <iostream>
#include <fstream>

namespace filesync::integration_test::debug {

    Debug::Debug(const std::string& testName,
        const std::string& server,
        const std::string& pathOnServer) :
            IntegrationTest(testName),
            server{server},
            pathOnServer{pathOnServer} {

    }

    void Debug::setup() {

    }

    void Debug::perform() {

    }

    void Debug::evaluate() {

    }

    void Debug::cleanUp() {

    }

}