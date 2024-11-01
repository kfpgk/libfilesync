#include <tests/system/FileSync.hpp>
#include <tests/config/config.h>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::system::FileSync test("FileSync System Test",
        LIBFILESYNC_TEST_FTP_SERVER,
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.run();

    return 0;
}