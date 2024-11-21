#include <libfilesync/FileSyncException.test.hpp>
#include <libfilesync/FileSyncException.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace filesync;
using namespace filesync::unit_test;
using namespace filesync::utility;

int main(int argc, char* argv[]) {

    FileSyncExceptionTest fileSyncExceptionTest;

    fileSyncExceptionTest.add_multiple_contexts();
    fileSyncExceptionTest.overwrite_error_message();

    Logger::getInstance().log(LogDomain::TestResult, "FileSyncException: passed", __FILE__, __LINE__);
    return 0;

}

namespace filesync::unit_test {

    void FileSyncExceptionTest::add_multiple_contexts() {

        std::stringstream expectedMessage;
        expectedMessage << "---" << std::endl;
        expectedMessage << "Outer context" << std::endl;
        expectedMessage << " Middle context" << std::endl;
        expectedMessage << "  Inner context" << std::endl;
        expectedMessage << "Error message" << std::endl;
        expectedMessage << "---" << std::endl;

        std::string message;

        try {
            try {
                try {
                    throw FileSyncException("Error message", "Inner context");
                } catch(FileSyncException& e) {
                    e.addContext("Middle context");
                    throw e;
                }
            } catch(FileSyncException& e) {
                e.addContext("Outer context");
                throw e;
            }
        } catch(const FileSyncException& e) {
            message = e.what();
        }
        std::cerr << "Message: " << std::endl;
        std::cerr << message << std::endl;
        std::cerr << "Expected Message: " << std::endl;
        std::cerr << expectedMessage.str() << std::endl;

        assert(expectedMessage.str() == message);

    }

    void FileSyncExceptionTest::overwrite_error_message() {

        std::string newErrorMessage("Modified error message");

        std::stringstream expectedMessage;
        expectedMessage << "---" << std::endl;
        expectedMessage << "Context" << std::endl;
        expectedMessage << newErrorMessage << std::endl;
        expectedMessage << "---" << std::endl;

        std::string message;

        try {
            try {
                throw FileSyncException("Original error message", "Context");
            } catch(FileSyncException& e) {
                e.setErrorMessage(newErrorMessage);
                throw e;
            }
        } catch(const FileSyncException& e) {
            message = e.what();
        }

        assert(expectedMessage.str() == message);

    }

}