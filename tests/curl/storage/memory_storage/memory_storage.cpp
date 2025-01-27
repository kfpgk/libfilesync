#include <tests/curl/storage/memory_storage/MemoryStorage.hpp>
#include <tests/config/config.h>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::curl::storage::memory_storage::MemoryStorage test(
        "curl upload and download to memory storage",
        LIBFILESYNC_TEST_FTP_SERVER,
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.run();

    return 0;
}