#include <libfilesync/core/sync_data/RemoteEntry.test.hpp>
#include <libfilesync/core/sync_data/RemoteEntry.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <cassert>

using namespace filesync::core::sync_data::unit_test;
using namespace filesync::utility;

int main(int argc, char* argv[]) {

    RemoteEntryTest remoteEntryTest;

    remoteEntryTest.get_path();
    remoteEntryTest.get_file_name();
    remoteEntryTest.get_location();

    Logger::getInstance().log(LogDomain::TestResult, "Class core::sync_data::RemoteEntry: passed",
        __FILE__, __LINE__);
    return 0;
}

namespace filesync::core::sync_data::unit_test {

    void RemoteEntryTest::get_path() {
        RemoteEntry remoteEntry("./sub_dir/dummy");

        assert("./sub_dir/dummy" == remoteEntry.getPath());

        Logger::getInstance().log(LogDomain::TestResult,
            "get_path() passed");
    }

    void RemoteEntryTest::get_file_name() {
        RemoteEntry remoteEntry("./sub_dir/dummy");

        assert("dummy" == remoteEntry.getFileName());

        Logger::getInstance().log(LogDomain::TestResult,
            "get_file_name() passed");
    }

    void RemoteEntryTest::get_location() {
        RemoteEntry remoteEntry("./sub_dir/dummy");

        assert("./sub_dir" == remoteEntry.getLocation());

        Logger::getInstance().log(LogDomain::TestResult,
            "get_location() passed");
    }

}