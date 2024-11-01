#include <tests/data/File/Changed.hpp>

using namespace filesync::integrationtest;

int main(int argc, char* argv[]) {

    data::file::Changed test("Test write detection on File/Entry object.");

    test.run();

    return 0;
}