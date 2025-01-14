#include <tests/curl/storage/file_storage/FileStorage.hpp>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::curl::storage::file_storage::FileStorage test(
        "curl file storage basic operations");

    test.run();

    return 0;
}