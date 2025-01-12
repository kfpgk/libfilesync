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
        inputFile1Name{"file1"},
        inputFile1Content{"file 1 content"},
        inputFile2Name{"file2"},
        inputFile2Content{"file 2 content"},
        flexibelFile1Name{"flexi_file1"},
        flexibelFile1Content{"flexi file1 content"} {

        TestCase swapTestCase {
            .name = "Swap two buffers",
            .perform = std::bind(&FileBuffer::performSwap, this),
            .evaluate = std::bind(&FileBuffer::evaluateSwap, this)
        };
        addTestCase(swapTestCase);

        TestCase copyConstruction {
            .name = "Test copy construction",
            .perform = std::bind(&FileBuffer::performCopyConstruction, this),
            .evaluate = std::bind(&FileBuffer::evaluateCopyConstruction, this)
        };
        addTestCase(copyConstruction);

        TestCase moveConstruction {
            .name = "Test move construction",
            .perform = std::bind(&FileBuffer::performMoveConstruction, this),
            .evaluate = std::bind(&FileBuffer::evaluateMoveConstruction, this)
        };
        addTestCase(moveConstruction);

        TestCase copyAssignment {
            .name = "Test copy assignment",
            .perform = std::bind(&FileBuffer::performCopyAssignment, this),
            .evaluate = std::bind(&FileBuffer::evaluateCopyAssignment, this)
        };
        addTestCase(copyAssignment);

        TestCase moveAssignment {
            .name = "Test move assignment",
            .perform = std::bind(&FileBuffer::performMoveAssignment, this),
            .evaluate = std::bind(&FileBuffer::evaluateMoveAssignment, this)
        };
        addTestCase(moveAssignment);

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
        std::ofstream file1(inputFile1Name);
        file1 << inputFile1Content << std::endl;
        std::ofstream file2(inputFile2Name);
        file2 << inputFile2Content << std::endl;
    }

    void FileBuffer::performSwap() {
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer1;
        filesync::FileBuffer buffer2;

        std::ifstream file1(inputFile1Name);
        buffer1.store(file1);

        std::ifstream file2(inputFile2Name);
        buffer2.store(file2);

        swap(buffer1, buffer2);

        result = 
            buffer1.isEqualTo(file2) && 
            buffer2.isEqualTo(file1);
    }

    void FileBuffer::evaluateSwap() {
        if (!result) {
            throw FileSyncException("Buffer content not as expected after swap",
                __FILE__, __LINE__);          
        }
    }

    void FileBuffer::performCopyConstruction() {
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer1;
        
        std::ifstream file1(inputFile1Name);
        buffer1.store(file1);

        filesync::FileBuffer buffer2{buffer1};

        result = 
            buffer2.isEqualTo(buffer1) && 
            buffer2.isEqualTo(file1);
    }

    void FileBuffer::evaluateCopyConstruction() {
        if (!result) {
            throw FileSyncException("Buffer contents of buffer1 and buffer2 do not "\
                "match file1 content after copy construction",
                __FILE__, __LINE__);          
        }
    }

    void FileBuffer::performMoveConstruction() {
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer1;
        
        std::ifstream file1(inputFile1Name);
        buffer1.store(file1);

        filesync::FileBuffer buffer2{std::move(buffer1)};

        resultOfIsEqual = buffer2.isEqualTo(file1);
        resultOfIsEmpty = buffer1.isEmpty();
    }

    void FileBuffer::evaluateMoveConstruction() {
        if (!resultOfIsEqual) {
            throw FileSyncException("buffer2 content does not match file1",
                __FILE__, __LINE__);          
        }
        if (!resultOfIsEmpty) {
            throw FileSyncException("buffer1 is not empty after being moved from.",
                __FILE__, __LINE__);          
        }
    }

    void FileBuffer::performCopyAssignment() {
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer1;
        
        std::ifstream file1(inputFile1Name);
        buffer1.store(file1);

        filesync::FileBuffer buffer2;
        buffer2 = buffer1;

        result = 
            buffer2.isEqualTo(buffer1) && 
            buffer2.isEqualTo(file1);
    }

    void FileBuffer::evaluateCopyAssignment() {
        if (!result) {
            throw FileSyncException("Copy assignment result not as expected",
                __FILE__, __LINE__);          
        }
    }

    void FileBuffer::performMoveAssignment() {
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer1;
        
        std::ifstream file1(inputFile1Name);
        buffer1.store(file1);

        filesync::FileBuffer buffer2;
        buffer2 = std::move(buffer1);

        resultOfIsEqual = buffer2.isEqualTo(file1);
        resultOfIsEmpty = buffer1.isEmpty();
    }

    void FileBuffer::evaluateMoveAssignment() {
        if (!resultOfIsEqual) {
            throw FileSyncException("buffer2 content does not match file1",
                __FILE__, __LINE__);          
        }
        if (!resultOfIsEmpty) {
            throw FileSyncException("buffer1 is not empty after being moved from.",
                __FILE__, __LINE__);          
        }
    }

    void FileBuffer::performStoreAndExtract() {
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer;
        std::ifstream readFile(inputFile1Name);
        buffer.store(readFile);

        std::ofstream writeFile(flexibelFile1Name);
        buffer.extractContentTo(writeFile);
    }

    void FileBuffer::evaluateStoreAndExtract() {
        if (!filesync::data::areEqual(inputFile1Name, flexibelFile1Name)) {
            throw FileSyncException("Data read from buffer is not equal to data that was stored"\
                " to the buffer", __FILE__, __LINE__);          
        }
    }

    void FileBuffer::performCheckEqualityPositive() {
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer;
        std::ifstream readFile(inputFile1Name);
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
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer;
        std::ifstream readFile(inputFile1Name);
        buffer.store(readFile);

        std::fstream anotherFile(flexibelFile1Name);
        anotherFile << flexibelFile1Content << std::endl;

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
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer;

        std::fstream anotherFile(flexibelFile1Name);
        anotherFile << flexibelFile1Content << std::endl;

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
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer;

        std::ifstream readFile(inputFile1Name);
        buffer.store(readFile);

        std::fstream anotherFile(flexibelFile1Name);
        anotherFile << flexibelFile1Content << std::endl;

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
        namespace filesync = filesync::core::sync_data::buffer;
        filesync::FileBuffer buffer;

        std::fstream anotherFile(flexibelFile1Name);

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