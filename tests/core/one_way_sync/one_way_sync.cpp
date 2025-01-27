#include <tests/core/one_way_sync/OneWaySync.hpp>
#include <tests/config/config.h>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::core::one_way_sync::OneWaySync test(
        "Test syncing in one direction for both local > remote and remote > local",
        LIBFILESYNC_TEST_FTP_SERVER,
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.run();

    return 0;
}