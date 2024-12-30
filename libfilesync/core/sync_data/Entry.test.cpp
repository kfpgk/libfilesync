#include <libfilesync/core/sync_data/Entry.test.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <cassert>
#include <variant>

using namespace filesync::core::sync_data::unit_test;

int main(int argc, char* argv[]) {

    EntryTest entryTest;

    entryTest.set_and_get_remote_entry();
    entryTest.get_default_remote_entry();
    entryTest.get_default_sync_in_progress();
    entryTest.set_sync_in_progress();
    entryTest.reset_sync_in_progress();
    entryTest.verify_remote_buf_location_type();
    entryTest.equality_operator_with_equals();
    entryTest.equality_operator_with_not_equals();

    Logger::getInstance().log(LogDomain::TestResult, "Class core::sync_data::Entry: passed",
        __FILE__, __LINE__);
    return 0;
}

namespace filesync::core::sync_data::unit_test {

    void EntryTest::set_and_get_remote_entry() {
        Entry entry("dummy");
        entry.setRemoteEntry("remote");

        assert("remote" == entry.getRemotePath());

        Logger::getInstance().log(LogDomain::TestResult,
            "set_and_get_remote_entry() passed");
    }

    void EntryTest::get_default_remote_entry() {
        Entry entry("dummy");

        assert("dummy" == entry.getRemotePath());

        Logger::getInstance().log(LogDomain::TestResult,
            "get_default_remote_entry() passed");
    }

    void EntryTest::get_default_sync_in_progress() {
        Entry entry("dummy");

        assert(false == entry.getSyncInProgress());

        Logger::getInstance().log(LogDomain::TestResult,
            "get_default_sync_in_progress() passed");
    }

    void EntryTest::set_sync_in_progress() {
        Entry entry("dummy");
        entry.setSyncInProgress();

        assert(true == entry.getSyncInProgress());

        Logger::getInstance().log(LogDomain::TestResult,
            "set_sync_in_progress() passed");
    }

    void EntryTest::reset_sync_in_progress() {
        Entry entry("dummy");
        entry.setSyncInProgress();
        entry.resetSyncInProgress();

        assert(false == entry.getSyncInProgress());

        Logger::getInstance().log(LogDomain::TestResult,
            "reset_sync_in_progress() passed");
    }

    void EntryTest::verify_remote_buf_location_type() {
        Entry entry("dummy");

        bool isFileSystemPath = 
            std::holds_alternative<
                std::reference_wrapper<const std::filesystem::path>>
                    (entry.getRemoteBufferHandle());

        assert(true == isFileSystemPath);

        Logger::getInstance().log(LogDomain::TestResult,
            "verify_remote_buf_location_type() passed");
    }

    void EntryTest::equality_operator_with_equals() {
        Entry entry("dummy");
        Entry entry2("dummy");

        assert(entry == entry2);

        Logger::getInstance().log(LogDomain::TestResult,
            "equality_operator_with_equals() passed");
    }

    void EntryTest::equality_operator_with_not_equals() {
        Entry entry("dummy");
        Entry entry2("dummy2");

        assert(!(entry == entry2));

        Logger::getInstance().log(LogDomain::TestResult,
            "equality_operator_with_not_equals() passed");
    }

}