#include <libfilesync/protocol/Client.test.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <cassert>

using namespace filesync::protocol::unit_test;
using namespace filesync::utility;

int main(int argc, char* argv[]) {

    ClientTest test;

    test.delete_remote_file_with_empty_path();
    test.download_file_with_empty_local_path();
    test.download_file_with_empty_remote_path();
    test.upload_file_with_empty_local_path();
    test.upload_file_with_empty_remote_path();

    Logger::getInstance().log(LogDomain::TestResult, "Class data::Directory: passed", __FILE__, __LINE__);
    return 0;
}

namespace filesync::protocol::unit_test {

    void ClientTest::delete_remote_file_with_empty_path() {
        ConcreteClient client;

        bool exceptionThrown = false;

        try {
            client.deleteOnServer("");
        } catch (...) {
            exceptionThrown = true;
        }

        assert(true == exceptionThrown);
    }

    void ClientTest::download_file_with_empty_local_path() {
        ConcreteClient client;

        bool exceptionThrown = false;

        try {
            client.download("", "dummy");
        } catch (...) {
            exceptionThrown = true;
        }

        assert(true == exceptionThrown);
    }

    void ClientTest::download_file_with_empty_remote_path() {
        ConcreteClient client;

        bool exceptionThrown = false;

        try {
            client.download("dummy", "");
        } catch (...) {
            exceptionThrown = true;
        }

        assert(true == exceptionThrown);
    }

    void ClientTest::upload_file_with_empty_local_path() {
        ConcreteClient client;

        bool exceptionThrown = false;

        try {
            client.upload("", "dummy");
        } catch (...) {
            exceptionThrown = true;
        }

        assert(true == exceptionThrown);
    }

    void ClientTest::upload_file_with_empty_remote_path() {
        ConcreteClient client;

        bool exceptionThrown = false;

        try {
            client.upload("dummy", "");
        } catch (...) {
            exceptionThrown = true;
        }

        assert(true == exceptionThrown);
    }    

}