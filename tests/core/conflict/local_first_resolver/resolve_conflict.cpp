#include <tests/core/conflict/local_first_resolver/ResolveConflict.hpp>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::core::conflict::local_first_resolver::ResolveConflict test(
        "Test resolving conflict by prioritising local content.");

    test.run();

    return 0;
}