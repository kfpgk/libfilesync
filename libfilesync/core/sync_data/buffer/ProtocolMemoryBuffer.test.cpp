#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.test.hpp>
#include <libfilesync/core/sync_data/buffer/ProtocolMemoryBuffer.hpp>
#include <libfilesync/protocol/memory/CharArray.hpp>
#include <libfilesync/protocol/memory/CharArrayHandle.hpp>
#include <libfilesync/utility/Logger.hpp>

#include <cassert>
#include <memory>
#include <span>
#include <sstream>
#include <string>
#include <vector>

using namespace filesync::utility;

int main(int argc, char* argv[]) {

    filesync::core::sync_data::buffer::unit_test::ProtocolMemoryBufferTest test;

    test.is_equal_to_with_empty_buffer();
    test.is_equal_to();
    test.is_not_equal_to();
    test.equal_start_but_buffer_is_larger();
    test.equal_start_but_input_is_larger();

    test.extract_content_to();

    test.store_into_empty_buffer();
    test.store_into_filled_buffer();

    test.get_non_existing_handle();
    test.get_valid_handle();
    test.get_handle_twice();
    
    Logger::getInstance().log(LogDomain::TestResult, 
        "Class ProtocolMemoryBuffer: passed", __FILE__, __LINE__);
    return 0;
}

namespace filesync::core::sync_data::buffer::unit_test {

    void ProtocolMemoryBufferTest::is_equal_to_with_empty_buffer() {

        ProtocolMemoryBuffer buffer;
        std::string inputString("My input string");
        std::stringstream inputStream(inputString);

        assert(!buffer.isEqualTo(inputStream));

        Logger::getInstance().log(LogDomain::TestResult,
            "is_equal_to_with_empty_buffer() passed");
    }

    void ProtocolMemoryBufferTest::is_equal_to() {

        std::string inputString("Hello");
        std::stringstream inputStream(inputString);

        ProtocolMemoryBuffer buffer(
            std::make_unique<protocol::memory::CharArrayHandle>(
                std::make_unique<protocol::memory::CharArray>(
                    std::vector<char>{'H', 'e', 'l', 'l', 'o'})));

        assert(buffer.isEqualTo(inputStream));

        Logger::getInstance().log(LogDomain::TestResult,
            "is_equal_to() passed");

    }

    void ProtocolMemoryBufferTest::is_not_equal_to() {

        std::string inputString("Hello");
        std::stringstream inputStream(inputString);

        ProtocolMemoryBuffer buffer(
            std::make_unique<protocol::memory::CharArrayHandle>(
                std::make_unique<protocol::memory::CharArray>(
                    std::vector<char>{'O', 'l', 'l', 'a', 'o'})));

        assert(!buffer.isEqualTo(inputStream));

        Logger::getInstance().log(LogDomain::TestResult,
            "is_not_equal_to() passed");

    }

    void ProtocolMemoryBufferTest::equal_start_but_input_is_larger() {

        std::string inputString("Hello again");
        std::stringstream inputStream(inputString);

        ProtocolMemoryBuffer buffer(
            std::make_unique<protocol::memory::CharArrayHandle>(
                std::make_unique<protocol::memory::CharArray>(
                    std::vector<char>{'H', 'e', 'l', 'l', 'o'})));

        assert(!buffer.isEqualTo(inputStream));

        Logger::getInstance().log(LogDomain::TestResult,
            "equal_start_but_input_is_larger() passed");

    }

    void ProtocolMemoryBufferTest::equal_start_but_buffer_is_larger() {

        std::string inputString("Hello");
        std::stringstream inputStream(inputString);

        ProtocolMemoryBuffer buffer(
            std::make_unique<protocol::memory::CharArrayHandle>(
                std::make_unique<protocol::memory::CharArray>(
                    std::vector<char>{'H', 'e', 'l', 'l', 'o', ' ', 'A', 'g', 'a', 'i', 'n'})));

        assert(!buffer.isEqualTo(inputStream));

        Logger::getInstance().log(LogDomain::TestResult,
            "equal_start_but_buffer_is_larger() passed");

    }

    void ProtocolMemoryBufferTest::extract_content_to() {

        std::string data;
        std::stringstream dataStream(data);

        ProtocolMemoryBuffer buffer(
            std::make_unique<protocol::memory::CharArrayHandle>(
                std::make_unique<protocol::memory::CharArray>(
                    std::vector<char>{'H', 'e', 'l', 'l', 'o'})));

        buffer.extractContentTo(dataStream);
        
        assert(buffer.isEqualTo(dataStream));

        Logger::getInstance().log(LogDomain::TestResult,
            "extract_content_to() passed");

    }

    void ProtocolMemoryBufferTest::store_into_empty_buffer() {

        ProtocolMemoryBuffer buffer;

        std::unique_ptr<protocol::memory::CharArrayHandle> handle =
            std::make_unique<protocol::memory::CharArrayHandle>(
                    std::make_unique<protocol::memory::CharArray>(
                        std::vector<char>{'H', 'e', 'l', 'l', 'o'}));

        buffer.store(std::move(handle));

        std::string compareString("Hello");
        std::stringstream compareStream(compareString);

        assert(handle == nullptr);
        assert(buffer.isEqualTo(compareStream));        

        Logger::getInstance().log(LogDomain::TestResult,
            "store_into_empty_buffer() passed");

    }

    void ProtocolMemoryBufferTest::store_into_filled_buffer() {

        ProtocolMemoryBuffer buffer(
            std::make_unique<protocol::memory::CharArrayHandle>(
                std::make_unique<protocol::memory::CharArray>(
                    std::vector<char>{'H', 'e', 'l', 'l', 'o'})));

        std::unique_ptr<protocol::memory::CharArrayHandle> handle =
            std::make_unique<protocol::memory::CharArrayHandle>(
                    std::make_unique<protocol::memory::CharArray>(
                        std::vector<char>{'A', 'g', 'a', 'i', 'n'}));

        buffer.store(std::move(handle));

        std::string compareString("Again");
        std::stringstream compareStream(compareString);

        assert(handle == nullptr);
        assert(buffer.isEqualTo(compareStream));  

        Logger::getInstance().log(LogDomain::TestResult,
            "store_into_filled_buffer() passed");

    }

    void ProtocolMemoryBufferTest::get_non_existing_handle() {
        
        ProtocolMemoryBuffer buffer;

        bool exceptionThrown = false;
        try {
            std::unique_ptr<protocol::memory::Handle<char>>& handle =
                buffer.getHandle();
        } catch (...) {
            exceptionThrown = true;
        }
        
        assert(exceptionThrown == true);

        Logger::getInstance().log(LogDomain::TestResult,
            "get_non_existing_handle() passed");

    }

    void ProtocolMemoryBufferTest::get_valid_handle() {

        std::vector<char> dataVector{'H', 'e', 'l', 'l', 'o'};

        ProtocolMemoryBuffer buffer(
            std::make_unique<protocol::memory::CharArrayHandle>(
                std::make_unique<protocol::memory::CharArray>(dataVector)));

        std::unique_ptr<protocol::memory::Handle<char>>& handle =
            buffer.getHandle();

        std::span<char> dataSpan = handle->data();

        assert(dataSpan.size() == dataVector.size());
        for (int i = 0; i < dataSpan.size(); i++) {
            assert(dataSpan[i] == dataVector[i]);
        }

        Logger::getInstance().log(LogDomain::TestResult,
            "get_valid_handle() passed");

    }

    void ProtocolMemoryBufferTest::get_handle_twice() {

        std::vector<char> dataVector{'H', 'e', 'l', 'l', 'o'};

        ProtocolMemoryBuffer buffer(
            std::make_unique<protocol::memory::CharArrayHandle>(
                std::make_unique<protocol::memory::CharArray>(dataVector)));

        std::unique_ptr<protocol::memory::Handle<char>>& handle1 =
            buffer.getHandle();

        std::unique_ptr<protocol::memory::Handle<char>>& handle2 =
            buffer.getHandle();

        std::span<char> dataSpan = handle2->data();

        assert(dataSpan.size() == dataVector.size());
        for (int i = 0; i < dataSpan.size(); i++) {
            assert(dataSpan[i] == dataVector[i]);
        }

        Logger::getInstance().log(LogDomain::TestResult,
            "get_handle_twice() passed");

    }

}