#include <libfilesync/curl/storage/CharBuffer.test.hpp>
#include <libfilesync/curl/storage/Charbuffer.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <cassert>
#include <cstddef>
#include <string>

using namespace filesync::curl;
using namespace filesync::utility;
using namespace filesync::utility::literals;

int main(int argc, char* argv[]) {

    storage::unit_test::CharBufferTest charBufferTest;

    charBufferTest.test_byte_array_and_string_write();

    Logger::getInstance().log(LogDomain::TestResult,
        "curl::storage::CharBufferTest: passed", __FILE__, __LINE__);
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

    void CharBufferTest::test_byte_array_and_string_write() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running test_byte_array_and_string_write()", __FILE__, __LINE__);
        
        CharBuffer data1;
        CharBuffer data2;

        std::string stringData {"test content"};
        char byteArrayData[] {"test content"};

        data1.write(stringData);
        data2.write(byteArrayData, 12);

        assert(data1 == data2);

    }

}