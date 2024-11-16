#include <tests/core/sync_data/file_buffer/FileBuffer.hpp>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::core::sync_data::file_buffer::FileBuffer test(
        "Test cync data file buffer store and retrieve");

    test.run();

    return 0;
}