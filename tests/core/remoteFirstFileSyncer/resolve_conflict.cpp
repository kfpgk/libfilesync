#include <tests/core/remoteFirstFileSyncer/ResolveConflict.hpp>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::core::remote_first_file_syncer::ResolveConflict test(
        "Test resolving conflict by prioritising remote content.");

    test.setExitOnFail(true);
    test.run();

    return 0;
}