#include <tests/data/CreateRecursiveEntries.hpp>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::data::CreateRecursiveEntries test("Create recursive entry structure");

    test.run();

    return 0;
}