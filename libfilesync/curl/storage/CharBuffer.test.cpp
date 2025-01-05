#include <libfilesync/curl/storage/CharBuffer.test.hpp>
#include <libfilesync/curl/storage/Charbuffer.hpp>
#include <libfilesync/utility/Logger.hpp>
#include <libfilesync/utility/Literals.hpp>

#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <ratio>
#include <span>
#include <string>

using namespace filesync::utility;
using namespace filesync::utility::literals;

int main(int argc, char* argv[]) {

    filesync::curl::storage::unit_test::CharBufferTest test;

    test.test_clear();
    test.test_get_size();
    test.test_byte_array_and_string_write();
    test.test_get_span();

    test.write_1MB_in_1KB_chunks_no_pre_alloc();
    test.write_1MB_in_1KB_chunks_with_pre_alloc();
    test.read_1MB_in_1KB_chunks();
    test.write_1MB_at_once_with_pre_alloc();
    test.read_1MB_at_once();

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

    void CharBufferTest::write_1MB_in_1KB_chunks_no_pre_alloc() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running write_1MB_in_1KB_chunks_no_pre_alloc()", __FILE__, __LINE__);
        
        CharBuffer buffer;

        char byteArrayData[1_KB] {"test content"};
        
        using namespace std::chrono;
        auto start = steady_clock::now();

        for (std::size_t i = 0; i < 1_KB; i++) {
            std::size_t remaining = 1_KB;
            while (remaining > 0) {
                std::size_t written = buffer.write(byteArrayData, remaining);
                remaining -= written;
            }
        }

        auto end = steady_clock::now();
        duration<float, std::milli> executionTime = end - start;

        Logger::getInstance().log(LogDomain::TestResult, 
            "Test time: " + std::to_string(executionTime.count()) + "ms");

        assert(executionTime < 200ms);

    }

    void CharBufferTest::write_1MB_in_1KB_chunks_with_pre_alloc() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running write_1MB_in_1KB_chunks_with_pre_alloc()", __FILE__, __LINE__);
        
        CharBuffer buffer(1_MB);

        char byteArrayData[1_KB] {"test content"};
        
        using namespace std::chrono;
        auto start = steady_clock::now();

        for (std::size_t i = 0; i < 1_KB; i++) {
            std::size_t remaining = 1_KB;
            while (remaining > 0) {
                std::size_t written = buffer.write(byteArrayData, remaining);
                remaining -= written;
            }
        }

        auto end = steady_clock::now();
        duration<float, std::milli> executionTime = end - start;

        Logger::getInstance().log(LogDomain::TestResult, 
            "Test time: " + std::to_string(executionTime.count()) + "ms");

        assert(executionTime < 10ms);

    }

    void CharBufferTest::read_1MB_in_1KB_chunks() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running read_1MB_in_1KB_chunks()", __FILE__, __LINE__);
        
        CharBuffer buffer(1_MB);

        char byteArrayData[1_KB] {"test content"};

        for (std::size_t i = 0; i < 1_KB; i++) {
            std::size_t remaining = 1_KB;
            while (remaining > 0) {
                std::size_t written = buffer.write(byteArrayData, remaining);
                remaining -= written;
            }
        }

        char* readToData = new char[1_MB];

        using namespace std::chrono;
        auto start = steady_clock::now();

        std::size_t remaining = 1_MB;
        std::size_t i = 0;
        while (remaining > 0) {
            std::size_t read = buffer.read(readToData + i * 1_KB, 1_KB);
            remaining -= read;
            i++;
        }

        auto end = steady_clock::now();
        duration<float, std::milli> executionTime = end - start;

        delete[] readToData;

        Logger::getInstance().log(LogDomain::TestResult, 
            "Test time: " + std::to_string(executionTime.count()) + "ms");

        assert(executionTime < 10ms);

    }

    void CharBufferTest::write_1MB_at_once_with_pre_alloc() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running write_1MB_at_once_with_pre_alloc()", __FILE__, __LINE__);
        
        CharBuffer buffer(1_MB);

        char* byteArrayData = new char[1_MB] {"test content"};

        using namespace std::chrono;
        auto start = steady_clock::now();

        std::size_t written = buffer.write(byteArrayData, 1_MB);

        auto end = steady_clock::now();
        duration<float, std::milli> executionTime = end - start;

        delete[] byteArrayData;
        Logger::getInstance().log(LogDomain::TestResult, 
            "Test time 1MB write: " + std::to_string(executionTime.count()) + "ms");

        assert(executionTime < 10ms);

    }

    void CharBufferTest::read_1MB_at_once() {
        Logger::getInstance().log(LogDomain::TestResult, 
            "Running read_1MB_at_once()", __FILE__, __LINE__);
        
        CharBuffer buffer(1_MB);

        char* byteArrayData = new char[1_MB] {"test content"};

        std::size_t written = buffer.write(byteArrayData, 1_MB);

        char* readToData = new char[1_MB];

        using namespace std::chrono;
        auto start = steady_clock::now();

        std::size_t read = buffer.read(readToData, 1_MB);

        auto end = steady_clock::now();
        duration<float, std::milli> executionTime = end - start;

        delete[] byteArrayData;
        delete[] readToData;
        Logger::getInstance().log(LogDomain::TestResult, 
            "Test time 1MB read: " + std::to_string(executionTime.count()) + "ms");

        assert(executionTime < 10ms);

    }    

}