#include <tests/core/buffered_sync/BufferedSync.hpp>
#include <tests/config/config.h>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::core::buffered_sync::BufferedSync test("Test buffered syncing.",
        LIBFILESYNC_TEST_FTP_SERVER,
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.run();

    return 0;
}