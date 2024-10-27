#include <tests/data/CreateRecursiveEntries.hpp>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::data::CreateRecursiveEntries test("Create recursive entry structure");

    test.setExitOnFail(true);
    test.run();

    return 0;
}