#include <tests/core/conflict/remote_first_resolver/ResolveConflict.hpp>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::core::conflict::remote_first_resolver::ResolveConflict test(
        "Test resolving conflict by prioritising remote content.");

    test.run();

    return 0;
}