#include <tests/core/sync_data/buffer/file_buffer/FileBuffer.hpp>
#include <libfilesync/core/sync_data/buffer/FileBuffer.hpp>
#include <libfilesync/data/Data.hpp>
#include <libfilesync/FileSyncException.hpp>

#include <iostream>
#include <fstream>
#include <cstdio>

namespace filesync::integration_test::core::sync_data::buffer::file_buffer {

    FileBuffer::FileBuffer(const std::string& testName) :
        IntegrationTest(testName),
        file1Name{"file1"},
        file1Content{"file 1 content"},
        file2Name{"file2"},
        file2Content{"file 2 content"} {

        TestCase storeAndExtract {
            .name = "Store to buffer and extract back to another file",
            .perform = std::bind(&FileBuffer::performStoreAndExtract, this),
            .evaluate = std::bind(&FileBuffer::evaluateStoreAndExtract, this)
        };
        addTestCase(storeAndExtract);

        TestCase checkEqualityPositive {
            .name = "Check equality with equal content",
            .perform = std::bind(&FileBuffer::performCheckEqualityPositive, this),
            .evaluate = std::bind(&FileBuffer::evaluateCheckEqualityPositive, this)
        };
        addTestCase(checkEqualityPositive);

        TestCase checkEqualityNegative {
            .name = "Check equality with different content",
            .perform = std::bind(&FileBuffer::performCheckEqualityNegative, this),
            .evaluate = std::bind(&FileBuffer::evaluateCheckEqualityNegative, this)
        };
        addTestCase(checkEqualityNegative);

        TestCase checkEqualityEmptyBuffer {
            .name = "Check equality with empty buffer",
            .perform = std::bind(&FileBuffer::performCheckEqualityEmptyBuffer, this),
            .evaluate = std::bind(&FileBuffer::evaluateCheckEqualityEmptyBuffer, this)
        };
        addTestCase(checkEqualityEmptyBuffer);

        TestCase checkEqualityEmptyCompData {
            .name = "Check equality with empty comparison data",
            .perform = std::bind(&FileBuffer::performCheckEqualityEmptyCompData, this),
            .evaluate = std::bind(&FileBuffer::evaluateCheckEqualityEmptyCompData, this)
        };
        addTestCase(checkEqualityEmptyCompData);

        TestCase checkEqualityEmptyBufferAndEmptyData {
            .name = "Check equality with empty buffer and compare data",
            .perform = std::bind(&FileBuffer::performCheckEqualityEmptyBufferAndEmptyData, this),
            .evaluate = std::bind(&FileBuffer::evaluateCheckEqualityEmptyBufferAndEmptyData, this)
        };
        addTestCase(checkEqualityEmptyBufferAndEmptyData);

    }

    void FileBuffer::setup() {
        std::ofstream file1(file1Name);
        file1 << file1Content << std::endl;
    }

    void FileBuffer::performStoreAndExtract() {
        filesync::core::sync_data::buffer::FileBuffer buffer;
        std::ifstream readFile(file1Name);
        buffer.store(readFile);

        std::ofstream writeFile(file2Name);
        buffer.extractContentTo(writeFile);
    }

    void FileBuffer::evaluateStoreAndExtract() {
        if (!filesync::data::areEqual(file1Name, file2Name)) {
            throw FileSyncException("Data read from buffer is not equal to data that was stored"\
                " to the buffer", __FILE__, __LINE__);          
        }
    }

    void FileBuffer::performCheckEqualityPositive() {
        filesync::core::sync_data::buffer::FileBuffer buffer;
        std::ifstream readFile(file1Name);
        buffer.store(readFile);

        resultOfIsEqual = false;
        resultOfIsEqual = buffer.isEqualTo(readFile);
    }

    void FileBuffer::evaluateCheckEqualityPositive() {
        if (resultOfIsEqual == false) {
            throw FileSyncException("Equality has been reported as false, while content was equal",
                __FILE__, __LINE__);          
        }
    }

    void FileBuffer::performCheckEqualityNegative() {
        filesync::core::sync_data::buffer::FileBuffer buffer;
        std::ifstream readFile(file1Name);
        buffer.store(readFile);

        std::fstream anotherFile(file2Name);
        anotherFile << file2Content << std::endl;

        resultOfIsEqual = true;
        resultOfIsEqual = buffer.isEqualTo(anotherFile);
    }

    void FileBuffer::evaluateCheckEqualityNegative() {
        if (resultOfIsEqual == true) {
            throw FileSyncException("Equality has been reported as true, while content was not equal",
                __FILE__, __LINE__);          
        }        
    }

    void FileBuffer::performCheckEqualityEmptyBuffer() {
        filesync::core::sync_data::buffer::FileBuffer buffer;

        std::fstream anotherFile(file2Name);
        anotherFile << file2Content << std::endl;

        resultOfIsEqual = true;
        resultOfIsEqual = buffer.isEqualTo(anotherFile);
    }

    void FileBuffer::evaluateCheckEqualityEmptyBuffer() {
        if (resultOfIsEqual == true) {
            throw FileSyncException("Equality has been reported as true, "\
                "while buffer was empty",
                __FILE__, __LINE__);          
        }        
    }

    void FileBuffer::performCheckEqualityEmptyCompData() {
        filesync::core::sync_data::buffer::FileBuffer buffer;
        std::ifstream readFile(file1Name);
        buffer.store(readFile);

        std::fstream anotherFile(file2Name);
        anotherFile << file2Content << std::endl;

        resultOfIsEqual = true;
        resultOfIsEqual = buffer.isEqualTo(anotherFile);
    }

    void FileBuffer::evaluateCheckEqualityEmptyCompData() {
        if (resultOfIsEqual == true) {
            throw FileSyncException("Equality has been reported as true, "\
                "while comparison data was empty",
                __FILE__, __LINE__);          
        }        
    }

    void FileBuffer::performCheckEqualityEmptyBufferAndEmptyData() {
        filesync::core::sync_data::buffer::FileBuffer buffer;

        std::fstream anotherFile(file2Name);

        resultOfIsEqual = true;
        resultOfIsEqual = buffer.isEqualTo(anotherFile);
    }

    void FileBuffer::evaluateCheckEqualityEmptyBufferAndEmptyData() {
        if (resultOfIsEqual == true) {
            throw FileSyncException("Equality has been reported as true, "\
                "while we except false for empty buffer",
                __FILE__, __LINE__);          
        }        
    }

}