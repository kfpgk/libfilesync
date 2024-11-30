#include <tests/core/sync_data/buffer/file_buffer/FileBuffer.hpp>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::core::sync_data::buffer::file_buffer::FileBuffer test(
        "Test cync data file buffer store and retrieve");

    test.run();

    return 0;
}