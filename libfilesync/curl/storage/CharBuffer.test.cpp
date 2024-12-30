#include <libfilesync/curl/storage/CharBuffer.test.hpp>
#include <libfilesync/curl/storage/Charbuffer.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Literals.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <cassert>
#include <cstddef>
#include <iostream>
#include <span>
#include <string>

using namespace filesync::curl;
using namespace filesync::utility;
using namespace filesync::utility::literals;

int main(int argc, char* argv[]) {

    storage::unit_test::CharBufferTest charBufferTest;

    charBufferTest.test_clear();
    charBufferTest.test_get_size();
    charBufferTest.test_byte_array_and_string_write();
    charBufferTest.test_get_span();

    Logger::getInstance().log(LogDomain::TestResult,
        "curl::storage::CharBuffer: passed", __FILE__, __LINE__);
    return 0;
}

namespace filesync::curl::storage::unit_test {

    void CharBufferTest::test_clear() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running test_clear()", __FILE__, __LINE__);
        
        CharBuffer data1;
        std::size_t sizeBefore = data1.getSize();
        std::string stringData {"test content"};

        data1.write(stringData);
        data1.clear();

        std::size_t sizeAfter = data1.getSize();

        assert(sizeBefore == sizeAfter);
    }

    void CharBufferTest::test_get_size() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running test_get_size()", __FILE__, __LINE__);
        
        CharBuffer data1;
        std::string stringData {"test content"};

        data1.write(stringData);

        assert(12 == data1.getSize());
    }

    void CharBufferTest::test_byte_array_and_string_write() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running test_byte_array_and_string_write()", __FILE__, __LINE__);
        
        CharBuffer data1;
        CharBuffer data2;

        std::string stringData {"test content"};
        char byteArrayData[] {"test content"};

        data1.write(stringData);
        
        std::size_t remaining = 12;
        while (remaining > 0) {
            std::size_t written = data2.write(byteArrayData, remaining);
            remaining -= written;
        }

        assert(data1 == data2);

    }

    void CharBufferTest::test_get_span() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running test_get_span()", __FILE__, __LINE__);

        std::string stringData {"Test content"};
        CharBuffer data1(stringData);

        data1.print();

        std::span span = data1.getSpan();

        assert(data1.getSize() == span.size());
        assert(span[0] == 'T');
        assert(span[1] == 'e');
        assert(span[2] == 's');
        assert(span[3] == 't');
        assert(span[4] == ' ');
        assert(span[5] == 'c');
        assert(span[6] == 'o');
        assert(span[7] == 'n');
        assert(span[8] == 't');
        assert(span[9] == 'e');
        assert(span[10] == 'n');
        assert(span[11] == 't');
        assert((void*)span.data() == (void*)data1.data);

    }

}