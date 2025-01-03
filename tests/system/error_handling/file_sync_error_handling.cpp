#include <tests/system/error_handling/FileSyncErrorHandling.hpp>
#include <tests/config/config.h>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::system::error_handling::FileSyncErrorHandling test(
            "FileSync Error Handling System Test ",
        LIBFILESYNC_TEST_FTP_SERVER,
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.run();

    return 0;
}