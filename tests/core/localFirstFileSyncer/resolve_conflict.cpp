#include <tests/core/localFirstFileSyncer/ResolveConflict.hpp>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::core::local_first_file_syncer::ResolveConflict test(
        "Test resolving conflict by prioritising local content.");

    test.run();

    return 0;
}