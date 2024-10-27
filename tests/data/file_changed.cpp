#include <tests/data/FileChanged.hpp>

using namespace filesync::integrationtest;

int main(int argc, char* argv[]) {

    data::FileChanged test("Test write detection on File/Entry object.");

    test.setExitOnFail(true);
    test.run();

    return 0;
}