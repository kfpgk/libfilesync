#include <tests/core/unbuffered_sync/UnbufferedSync.hpp>
#include <tests/config/config.h>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::core::unbuffered_sync::UnbufferedSync test("Test unbuffered syncing.",
        LIBFILESYNC_TEST_FTP_SERVER,
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.run();

    return 0;
}