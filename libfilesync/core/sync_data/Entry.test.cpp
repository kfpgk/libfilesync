#include <libfilesync/core/sync_data/Entry.test.hpp>
#include <libfilesync/core/sync_data/Entry.hpp>
#include <libfilesync/core/sync_data/buffer/Buffer.hpp>
#include <libfilesync/core/sync_data/buffer/CharArrayMemoryBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <cassert>
#include <variant>

using namespace filesync::utility;

int main(int argc, char* argv[]) {

    filesync::core::sync_data::unit_test::EntryTest test;

    test.set_and_get_remote_entry();
    test.get_default_remote_entry();
    test.get_default_sync_in_progress();

    test.set_sync_in_progress();
    test.reset_sync_in_progress();

    test.equality_operator_with_equals();
    test.equality_operator_with_not_equals();

    test.construct_with_file_buffers();
    test.construct_with_memory_buffers();
    test.construct_remote_with_char_array_buffer();
    test.construct_previous_with_protocol_buffer();

    test.set_file_buffers();
    test.set_memory_buffers();
    test.set_previous_with_protocol_buffer();
    test.set_remote_with_char_array_buffer();

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

    void EntryTest::construct_with_file_buffers() {

        Entry entry("dummy1", buffer::FileBuffer{}, buffer::FileBuffer{});

        Logger::getInstance().log(LogDomain::TestResult,
            "construct_with_file_buffers() passed");
    }

    void EntryTest::construct_with_memory_buffers() {

        Entry entry("dummy1", 
            buffer::ProtocolMemoryBuffer{},
            buffer::CharArrayMemoryBuffer{});

        Logger::getInstance().log(LogDomain::TestResult,
            "construct_with_protocol_memory_buffers() passed");
    }

    void EntryTest::construct_remote_with_char_array_buffer() {

        bool exceptionThrown = false;
        try {
            Entry entry("dummy1", 
                buffer::CharArrayMemoryBuffer{},
                buffer::CharArrayMemoryBuffer{});
        } catch (...) {
            exceptionThrown = true;
        }

        assert(exceptionThrown == true);

        Logger::getInstance().log(LogDomain::TestResult,
            "construct_remote_with_char_array_buffer() passed");
    }

    void EntryTest::construct_previous_with_protocol_buffer() {

        bool exceptionThrown = false;
        try {
            Entry entry("dummy1", 
                buffer::ProtocolMemoryBuffer{},
                buffer::ProtocolMemoryBuffer{});
        } catch (...) {
            exceptionThrown = true;
        }

        assert(exceptionThrown == true);

        Logger::getInstance().log(LogDomain::TestResult,
            "construct_previous_with_protocol_buffer() passed");
    }

    void EntryTest::set_file_buffers() {

        Entry entry("dummy1");
        entry.setBuffers(
            buffer::FileBuffer{},
            buffer::FileBuffer{});

        Logger::getInstance().log(LogDomain::TestResult,
            "set_file_buffers() passed");
    }

    void EntryTest::set_memory_buffers() {

        Entry entry("dummy1");
        entry.setBuffers(
            buffer::ProtocolMemoryBuffer{},
            buffer::CharArrayMemoryBuffer{});

        Logger::getInstance().log(LogDomain::TestResult,
            "set_memory_buffers() passed");
    }

    void EntryTest::set_remote_with_char_array_buffer() {

        Entry entry("dummy1");

        bool exceptionThrown = false;
        try {
            entry.setBuffers(
                buffer::CharArrayMemoryBuffer{},
                buffer::CharArrayMemoryBuffer{});
        } catch (...) {
            exceptionThrown = true;
        }

        assert(exceptionThrown == true);

        Logger::getInstance().log(LogDomain::TestResult,
            "set_remote_with_char_array_buffer() passed");
    }

    void EntryTest::set_previous_with_protocol_buffer() {

        Entry entry("dummy1");

        bool exceptionThrown = false;
        try {
            entry.setBuffers(
                buffer::ProtocolMemoryBuffer{},
                buffer::ProtocolMemoryBuffer{});
        } catch (...) {
            exceptionThrown = true;
        }

        assert(exceptionThrown == true);

        Logger::getInstance().log(LogDomain::TestResult,
            "set_previous_with_protocol_buffer() passed");
    }

}