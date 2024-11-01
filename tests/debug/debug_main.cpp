#include <tests/debug/Debug.hpp>
#include <tests/config/config.h>

namespace test = filesync::integrationtest;

int main (int argc, char* argv[]) {

    test::debug::Debug test("Test debug environment",
        LIBFILESYNC_TEST_FTP_SERVER,
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.setKeepSandBox(true);
    test.run();

    return 0;
}